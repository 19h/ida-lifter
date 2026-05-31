#!/usr/bin/env python3
"""Build and run the AVX torture corpus across many seeds, hunting for lifter
INTERRs / decompilation failures and reporting any instruction left as __asm.

For each seed it: generates a C corpus, compiles it to a shared object, runs
idump with the lifter, and scans the pseudocode for
  * INTERR<nnnnn>            -> a hard decompiler internal error (the target)
  * a function that failed to decompile (success rate < 100%)
  * __asm { ... }            -> an instruction the lifter did not lift
Findings are reproducible: rerun with the printed --seed/--funcs.

Usage:
    run_torture.py --seeds 20 --funcs 400
    run_torture.py --seed 7 --funcs 800 --keep   # one seed, keep artifacts
"""
from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

HERE = Path(__file__).resolve().parent
ANSI = re.compile(r"\x1b\[[0-9;]*m")
ASM = re.compile(r"__asm\s*\{\s*([^}\n;]+)")
INTERR = re.compile(r"INTERR[ :]?\s*(\d+)", re.IGNORECASE)
TOTAL = re.compile(r"Total functions:\s*(\d+)")
OKRE = re.compile(r"Decompiled OK:\s*(\d+)")
RATE = re.compile(r"Success rate:\s*([0-9.]+)")


def sh(cmd, **kw):
    return subprocess.run(cmd, shell=True, text=True, capture_output=True, **kw)


def _idump_scan(idump, sofile, outfile, keep, interesting_only=True):
    dump = sh(f"{idump} --plugin lifter --pseudo-only --no-color {sofile}")
    text = ANSI.sub("", dump.stdout)
    Path(outfile).write_text(text)
    ok = int(m.group(1)) if (m := OKRE.search(text)) else -1
    rate = float(m.group(1)) if (m := RATE.search(text)) else -1.0
    interrs = INTERR.findall(text)
    asms = Counter(b.strip().split()[0].lower() for b in ASM.findall(text) if b.strip())
    if not keep and interesting_only and not interrs and rate >= 100.0 and not asms:
        Path(outfile).unlink(missing_ok=True)
    return ok, rate, interrs, asms


def run_seed(seed, funcs, flags, idump, keep, cc):
    base = HERE / f"_t{seed}"
    ok_total, rate_min, interrs, asms = 0, 100.0, [], Counter()
    # --- intrinsic corpus (C) ---
    cfile, cso, cout = f"{base}.c", f"{base}.so", f"{base}.out"
    gen = sh(f"python3 {HERE}/gen_torture.py --funcs {funcs} --seed {seed} --out {cfile}")
    if gen.returncode != 0:
        return {"seed": seed, "stage": "gen-c", "err": gen.stderr.strip()}
    res = sh(f"{cc} -arch x86_64 -O1 -fPIC -shared {flags} {cfile} -o {cso}")
    if res.returncode != 0:
        return {"seed": seed, "stage": "compile-c",
                "err": "\n".join(l for l in res.stderr.splitlines() if "error:" in l)[:2000]}
    o, r, ie, am = _idump_scan(idump, cso, cout, keep)
    ok_total += max(o, 0); rate_min = min(rate_min, r); interrs += [("c", x) for x in ie]; asms += am
    if not keep:
        for f in (cfile, cso): Path(f).unlink(missing_ok=True)

    # --- raw-asm fringe corpus ---
    afile, aso, aout = f"{base}.s", f"{base}_asm.so", f"{base}_asm.out"
    gen = sh(f"python3 {HERE}/gen_asm_torture.py --funcs {funcs} --seed {seed} --out {afile}")
    if gen.returncode != 0:
        return {"seed": seed, "stage": "gen-asm", "err": gen.stderr.strip()}
    res = sh(f"{cc} -arch x86_64 -shared -fPIC {flags} {afile} -o {aso}")
    if res.returncode != 0:
        return {"seed": seed, "stage": "assemble",
                "err": "\n".join(l for l in res.stderr.splitlines() if "rror" in l)[:2000]}
    o, r, ie, am = _idump_scan(idump, aso, aout, keep)
    ok_total += max(o, 0); rate_min = min(rate_min, r); interrs += [("asm", x) for x in ie]; asms += am
    if not keep:
        for f in (afile, aso): Path(f).unlink(missing_ok=True)

    return {"seed": seed, "stage": "ok", "total": ok_total, "ok": ok_total,
            "rate": rate_min, "interrs": interrs, "asms": asms}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--seeds", type=int, default=10, help="run seeds 1..N")
    ap.add_argument("--seed", type=int, default=None, help="single seed")
    ap.add_argument("--funcs", type=int, default=400)
    ap.add_argument("--idump", default="idump")
    ap.add_argument("--cc", default=os.environ.get("CC", "clang"),
                    help="C compiler (default: $CC or clang)")
    ap.add_argument("--keep", action="store_true")
    args = ap.parse_args()

    flags = (HERE / "flags.txt").read_text().strip() if (HERE / "flags.txt").exists() else \
        "-mavx2 -mfma -mavx512f -mavx512bw -mavx512dq -mavx512vl -mavx512cd -mavx512fp16"

    seeds = [args.seed] if args.seed is not None else list(range(1, args.seeds + 1))
    total_funcs = 0
    interr_seeds = []
    all_asm = Counter()
    bad = []
    for s in seeds:
        r = run_seed(s, args.funcs, flags, args.idump, args.keep, args.cc)
        if r["stage"] != "ok":
            print(f"[seed {s}] {r['stage']} FAILED:\n{r['err']}")
            bad.append(s)
            continue
        total_funcs += r["ok"] if r["ok"] > 0 else 0
        tag = ""
        if r["interrs"]:
            interr_seeds.append((s, r["interrs"]))
            tag = f"  *** INTERR {r['interrs']} ***"
        if r["rate"] >= 0 and r["rate"] < 100.0:
            tag += f"  *** rate {r['rate']}% ***"
            if s not in [x[0] for x in interr_seeds]:
                interr_seeds.append((s, ["rate<100"]))
        all_asm.update(r["asms"])
        print(f"[seed {s}] funcs={r['ok']}/{r['total']} rate={r['rate']}% "
              f"asm={sum(r['asms'].values())}{tag}")

    print("\n==== TORTURE SUMMARY ====")
    print(f"seeds run:           {len(seeds)}")
    print(f"functions decompiled:{total_funcs}")
    print(f"compile/gen failures:{len(bad)} {bad if bad else ''}")
    print(f"INTERR / failures:   {len(interr_seeds)}")
    for s, e in interr_seeds:
        print(f"   seed {s}: {e}  (artifacts kept: _t{s}.c/.so/.out)")
    if all_asm:
        print(f"unlifted __asm mnemonics ({sum(all_asm.values())} occ):")
        for mn, c in all_asm.most_common():
            print(f"   {mn:24} {c}")
    else:
        print("unlifted __asm:      none")
    # fail the run if any INTERR/decompile failure
    return 1 if interr_seeds or bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
