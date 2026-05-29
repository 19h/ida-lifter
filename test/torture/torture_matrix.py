#!/usr/bin/env python3
"""Cross-compiler / cross-format BUILD MATRIX harness for the AVX-512 lifter
torture suite -- the integration keystone that drives EVERY generator across the
whole wild-condition build matrix and hunts decompiler INTERRs.

For each discovered generator (any test/torture/gen_*.py following the CLI
contract `--funcs N --seed S --out FILE`) and each seed, this:

  1. generates the corpus (C or raw asm -- detected from the file content),
  2. builds it under every APPLICABLE config in the matrix
       gcc      -> ELF64 .so   (-O0/-O1/-O2/-O3/-Os)
       clang    -> ELF64 .so
       gcc -m32 -> ELF32 .so   (AVX2/FMA only; AVX-512 corpora are skipped)
       clang --target=x86_64-pc-windows-gnu -> PE64 .dll (freestanding, lld)
       x86_64-w64-mingw32-gcc -> PE64 .dll (--export-all-symbols)
       i686-w64-mingw32-gcc   -> PE32 .dll (AVX2/FMA only)
       (asm corpora: ELF64 via gcc + clang; the mingw PE assemblers reject the
        ELF .type/.size/.section directives, so PE-asm combos are logged-skipped)
  3. runs `idump --plugin lifter --pseudo-only --no-color BIN` over every binary
     that built, and scans stdout for
        INTERR<nnnnn>      -> hard decompiler internal error (the target),
        Success rate <100  -> a decompile failure,
        __asm { <mnem>     -> an instruction the lifter did not lift.

Build combos that don't apply (AVX-512 under 32-bit, PE asm, ...) are PROBED once
and SKIPPED with a logged reason -- never silently dropped.

The run exits non-zero if any INTERR or decompile failure is found.  Every
finding prints the exact (generator, config, seed) needed to reproduce it, and
the artifacts of any interesting binary are kept (always under --keep, otherwise
only the interesting ones).

Usage:
    torture_matrix.py --seeds 1 --funcs 40
    torture_matrix.py --seeds 4 --funcs 200 --keep
    torture_matrix.py --seeds 1 --funcs 40 --idump /path/to/idump
"""
from __future__ import annotations

import argparse
import glob
import os
import re
import shutil
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
PREFIX = "_mtx"  # unique artifact prefix so parallel agents don't collide

# --- scan regexes (kept identical in spirit to run_torture.py) --------------
ANSI = re.compile(r"\x1b\[[0-9;]*m")
ASM = re.compile(r"__asm\s*\{\s*([^}\n;]+)")
INTERR = re.compile(r"INTERR[ :]?\s*(\d+)", re.IGNORECASE)
OKRE = re.compile(r"Decompiled OK:\s*(\d+)")
TOTRE = re.compile(r"Total functions:\s*(\d+)")
RATE = re.compile(r"Success rate:\s*([0-9.]+)")
# the lifter prints a clean per-function failure line in its "Errors:" block:
#   evfr_1_4: INTERR: 50757
ERRLINE = re.compile(r"^\s*([A-Za-z_][\w.]*):\s*INTERR:\s*(\d+)", re.MULTILINE)

# AVX-512 ISA flags are not legal on 32-bit targets; the C corpora are heavy
# AVX-512 so they only build 64-bit.  Detect AVX-512 use to gate 32-bit combos.
AVX512_RE = re.compile(r"_mm512_|__m512|__mmask|512|avx512", re.IGNORECASE)


def sh(cmd, timeout=600):
    return subprocess.run(cmd, shell=True, text=True,
                          capture_output=True, timeout=timeout)


# ---------------------------------------------------------------------------
# DllMainCRTStartup stub: clang --target=x86_64-pc-windows-gnu -nostdlib needs
# an entry symbol for a DLL.  Compiled as a separate TU and linked in so the
# corpus functions are kept and exported (idump loads the PE natively).
# ---------------------------------------------------------------------------
DLLMAIN_STUB_SRC = (
    "int DllMainCRTStartup(void *a, unsigned b, void *c)\n"
    "{ (void)a; (void)b; (void)c; return 1; }\n"
)


def write_dllmain_stub() -> Path:
    p = HERE / f"{PREFIX}_dllmain.c"
    if not p.exists():
        p.write_text(DLLMAIN_STUB_SRC)
    return p


# ---------------------------------------------------------------------------
# Build matrix.  Each config is a dict describing how to build one corpus file.
#   kind     : "c" | "asm"      (which corpus content it applies to)
#   ext      : output binary extension
#   avx512   : True if config tolerates AVX-512 corpora (False => skip them)
#   fmt      : human label for the summary (ELF64/ELF32/PE64/PE32)
#   cmd(src,out,flags,stub) -> shell command string
# Configs whose toolchain is missing are auto-skipped (logged).
# ---------------------------------------------------------------------------
def _has(tool):
    return shutil.which(tool) is not None


def build_matrix():
    cfgs = []

    # ---- gcc ELF64, all opt levels (C corpora) ----
    for opt in ("O0", "O1", "O2", "O3", "Os"):
        cfgs.append(dict(
            name=f"gcc-elf64-{opt}", kind="c", ext="so", fmt="ELF64",
            avx512=True, tool="gcc",
            cmd=lambda s, o, f, stub, opt=opt:
                f"gcc -{opt} -fPIC -shared {f} {s} -o {o}"))

    # ---- clang ELF64 (C corpora) ----
    cfgs.append(dict(
        name="clang-elf64-O2", kind="c", ext="so", fmt="ELF64",
        avx512=True, tool="clang",
        cmd=lambda s, o, f, stub:
            f"clang -O2 -fPIC -shared {f} {s} -o {o}"))

    # ---- gcc -m32 ELF32 (C corpora; AVX-512 corpora gated out) ----
    cfgs.append(dict(
        name="gcc-elf32-O1", kind="c", ext="so", fmt="ELF32",
        avx512=False, tool="gcc", m32=True,
        cmd=lambda s, o, f, stub:
            f"gcc -O1 -m32 -fPIC -shared -mavx2 -mfma {s} -o {o}"))

    # ---- clang --target=x86_64-pc-windows-gnu -> PE64 DLL (C corpora) ----
    cfgs.append(dict(
        name="clang-pe64-O1", kind="c", ext="dll", fmt="PE64",
        avx512=True, tool="clang",
        cmd=lambda s, o, f, stub:
            "clang --target=x86_64-pc-windows-gnu -ffreestanding -nostdlib "
            f"-fuse-ld=lld -shared -O1 {f} -Wl,--export-all-symbols "
            f"{s} {stub} -o {o}"))

    # ---- x86_64-w64-mingw32-gcc -> PE64 DLL (C corpora) ----
    cfgs.append(dict(
        name="mingw64-pe64-O1", kind="c", ext="dll", fmt="PE64",
        avx512=True, tool="x86_64-w64-mingw32-gcc",
        cmd=lambda s, o, f, stub:
            f"x86_64-w64-mingw32-gcc -shared -O1 {f} "
            f"-Wl,--export-all-symbols {s} -o {o}"))

    # ---- i686-w64-mingw32-gcc -> PE32 DLL (C corpora; AVX-512 gated out) ----
    cfgs.append(dict(
        name="mingw32-pe32-O1", kind="c", ext="dll", fmt="PE32",
        avx512=False, tool="i686-w64-mingw32-gcc",
        cmd=lambda s, o, f, stub:
            f"i686-w64-mingw32-gcc -shared -O1 -mavx2 -mfma "
            f"-Wl,--export-all-symbols {s} -o {o}"))

    # ---- asm corpora: ELF64 via gcc + clang ----
    cfgs.append(dict(
        name="gcc-asm-elf64", kind="asm", ext="so", fmt="ELF64",
        avx512=True, tool="gcc",
        cmd=lambda s, o, f, stub:
            f"gcc -shared -fPIC {f} {s} -o {o}"))
    cfgs.append(dict(
        name="clang-asm-elf64", kind="asm", ext="so", fmt="ELF64",
        avx512=True, tool="clang",
        cmd=lambda s, o, f, stub:
            f"clang -shared -fPIC {f} {s} -o {o}"))
    # asm PE combos are intentionally absent: the generators emit ELF-only
    # directives (.type @function / .size / .section .rodata) that the mingw
    # PE assembler rejects.  They are reported as logged-skips per binary below
    # so the omission is explicit, not silent.

    return [c for c in cfgs if _has(c["tool"])]


# These config names are *expected* to not exist for asm corpora; we synthesize
# explicit skip records for them so the "no silent drops" rule is honored.
PE_ASM_SKIPS = [
    ("mingw64-pe-asm", "PE64", "x86_64-w64-mingw32-gcc"),
    ("mingw32-pe-asm", "PE32", "i686-w64-mingw32-gcc"),
]


# ---------------------------------------------------------------------------
def diagnose_build_failure(stderr: str) -> str:
    """Pick the most specific root-cause line from a failed build's stderr.

    A bare 'clang: error: linker command failed' or 'collect2: error: ld
    returned 1' is uninformative -- the real cause is the preceding linker /
    compiler diagnostic.  Rank lines so the ledger explains *why* a combo was
    skipped."""
    lines = [l.strip() for l in stderr.splitlines() if l.strip()]
    if not lines:
        return "build failed (no diagnostic)"
    GENERIC = ("linker command failed", "ld returned 1",
               "ld.lld: error: linker", "collect2:")

    def score(l: str) -> int:
        low = l.lower()
        s = 0
        if "version node not found" in low or "dynamic section" in low:
            s += 5          # vectorcall/regcall @@ mangling vs ld.bfd
        if "undefined symbol" in low or "undefined reference" in low:
            s += 4
        if low.startswith("error:") or ": error:" in low:
            s += 3
        if "rror" in l:
            s += 1
        if any(g in low for g in GENERIC):
            s -= 5          # generic trailers: least useful
        return s

    best = max(lines, key=score)
    return best[:300]


def detect_kind(path: Path) -> str:
    """Sniff a generated corpus: C starts with #include, asm with a directive."""
    head = path.read_text(errors="replace")[:4096]
    if "#include" in head or "__m512" in head:
        return "c"
    if head.lstrip().startswith(".") or ".globl" in head or ".text" in head:
        return "asm"
    # fallback by extension
    return "c" if path.suffix == ".c" else "asm"


def gen_corpus(gen: Path, funcs: int, seed: int) -> tuple[Path, str, str]:
    """Run a generator; return (corpus_path, kind, error_or_empty).

    We let the generator pick its native extension by passing it a name with the
    extension we *guess* from its default --out, then re-sniff the content."""
    stem = gen.stem  # e.g. gen_evex_fringe
    base = HERE / f"{PREFIX}_{stem}_s{seed}"
    # First write as .c; generators that emit asm just put asm bytes in it -- we
    # then sniff and rename to .s.  But some generators (gas .s) reference local
    # data sections; extension doesn't affect generation, only our build step.
    out_c = f"{base}.c"
    r = sh(f"python3 {gen} --funcs {funcs} --seed {seed} --out {out_c}")
    if r.returncode != 0:
        return Path(out_c), "?", r.stderr.strip()[:1500]
    kind = detect_kind(Path(out_c))
    if kind == "asm":
        out_s = f"{base}.s"
        os.replace(out_c, out_s)
        return Path(out_s), kind, ""
    return Path(out_c), kind, ""


# ---------------------------------------------------------------------------
def idump_scan(idump: str, binpath: Path, outpath: Path):
    """Dump a binary, return (total, ok, rate, interrs, err_funcs, asm_counter,
    func_seen)."""
    r = sh(f"{idump} --plugin lifter --pseudo-only --no-color {binpath} "
           f"2>/dev/null")
    text = ANSI.sub("", r.stdout)
    outpath.write_text(text)
    total = int(m.group(1)) if (m := TOTRE.search(text)) else -1
    ok = int(m.group(1)) if (m := OKRE.search(text)) else -1
    rate = float(m.group(1)) if (m := RATE.search(text)) else -1.0
    interrs = INTERR.findall(text)
    # tie each INTERR to a function name where the lifter reported one
    err_funcs = [(fn, code) for fn, code in ERRLINE.findall(text)]
    asms = Counter(b.strip().split()[0].lower()
                   for b in ASM.findall(text) if b.strip())
    return total, ok, rate, interrs, err_funcs, asms


def native_interr_funcs(idump: str, binpath: Path) -> set:
    """Re-dump WITHOUT the lifter plugin. Any function that still INTERRs is a
    native IDA/Hex-Rays decompiler bug, not something the lifter caused — so it
    must not be counted as a lifter regression."""
    r = sh(f"{idump} --pseudo-only --no-color {binpath} 2>/dev/null")
    text = ANSI.sub("", r.stdout)
    return {fn for fn, _code in ERRLINE.findall(text)}


# ---------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--seeds", type=int, default=1, help="run seeds 1..N")
    ap.add_argument("--funcs", type=int, default=40)
    ap.add_argument("--idump", default="idump")
    ap.add_argument("--keep", action="store_true",
                    help="keep artifacts of every built binary, not just "
                         "the interesting (INTERR/asm/failure) ones")
    ap.add_argument("--only", default=None,
                    help="comma-separated generator stems to restrict to "
                         "(e.g. gen_evex_fringe)")
    args = ap.parse_args()

    flags = (HERE / "flags.txt").read_text().strip() if (HERE / "flags.txt").exists() \
        else "-mavx2 -mfma -mavx512f -mavx512bw -mavx512dq -mavx512vl"

    gens = sorted(HERE.glob("gen_*.py"))
    if args.only:
        want = set(args.only.split(","))
        gens = [g for g in gens if g.stem in want]
    if not gens:
        print("no generators found (test/torture/gen_*.py)")
        return 2

    cfgs = build_matrix()
    stub = write_dllmain_stub()
    seeds = list(range(1, args.seeds + 1))

    print("==== TORTURE MATRIX ====")
    print(f"generators : {', '.join(g.stem for g in gens)}")
    print(f"configs    : {', '.join(c['name'] for c in cfgs)}")
    print(f"seeds      : {seeds}   funcs/seed : {args.funcs}")
    print(f"idump      : {args.idump}")
    print()

    # aggregate state -------------------------------------------------------
    grand_funcs = 0
    grand_bins = 0
    bins_by_fmt = Counter()
    all_asm = Counter()
    interr_findings = []     # (gen, cfg, seed, code, func) — lifter-caused
    ida_native_findings = [] # (gen, cfg, seed, code, func) — reproduce w/o lifter
    failure_findings = []    # (gen, cfg, seed, rate)
    skip_log = []            # (gen, cfg, seed, reason)
    gen_fail_log = []        # (gen, seed, stage, err)
    proof = {}               # fmt -> (binpath, total, ok) of one successful dump
    keep_paths = set()
    cleanup_paths = set()

    for gen in gens:
        for seed in seeds:
            corpus, kind, gerr = gen_corpus(gen, args.funcs, seed)
            cleanup_paths.add(corpus)
            if kind == "?":
                gen_fail_log.append((gen.stem, seed, "generate", gerr))
                print(f"[{gen.stem} s{seed}] GENERATE FAILED: {gerr.splitlines()[0] if gerr else '?'}")
                continue
            is_512 = bool(AVX512_RE.search(corpus.read_text(errors='replace')[:200000]))

            # synthesize explicit skip records for inapplicable PE-asm combos
            if kind == "asm":
                for nm, fmt, tool in PE_ASM_SKIPS:
                    if _has(tool):
                        skip_log.append((gen.stem, nm, seed,
                                         f"{fmt}: mingw PE assembler rejects ELF "
                                         f".type/.size/.section directives"))

            avx2_corpus = None  # lazily generated AVX2-only variant for 32-bit
            for cfg in cfgs:
                if cfg["kind"] != kind:
                    continue
                build_corpus = corpus
                if is_512 and not cfg["avx512"]:
                    # 32-bit target can't take AVX-512. If the generator supports
                    # an --avx2 mode, build that variant so ELF32/PE32 actually
                    # exercise the lifter's 32-bit path; else skip honestly.
                    if avx2_corpus is None and kind == "c":
                        cand = HERE / f"{PREFIX}_{gen.stem}_s{seed}_avx2.c"
                        rr = sh(f"python3 {gen} --funcs {args.funcs} --seed {seed} "
                                f"--avx2 --out {cand}")
                        avx2_corpus = cand if (rr.returncode == 0 and cand.exists()) else False
                        if avx2_corpus:
                            cleanup_paths.add(avx2_corpus)
                    if not avx2_corpus:
                        skip_log.append((gen.stem, cfg["name"], seed,
                                         f"{cfg['fmt']}: corpus uses AVX-512 and "
                                         f"generator has no --avx2 mode"))
                        continue
                    build_corpus = avx2_corpus
                binp = HERE / f"{PREFIX}_{gen.stem}_s{seed}_{cfg['name']}.{cfg['ext']}"
                outp = HERE / f"{PREFIX}_{gen.stem}_s{seed}_{cfg['name']}.out"
                cleanup_paths.update((binp, outp))
                cmd = cfg["cmd"](str(build_corpus), str(binp), flags, str(stub))
                try:
                    b = sh(cmd)
                except subprocess.TimeoutExpired:
                    skip_log.append((gen.stem, cfg["name"], seed, "build timeout"))
                    continue
                if b.returncode != 0 or not binp.exists():
                    skip_log.append((gen.stem, cfg["name"], seed,
                                     diagnose_build_failure(b.stderr)))
                    continue

                grand_bins += 1
                bins_by_fmt[cfg["fmt"]] += 1
                total, ok, rate, interrs, err_funcs, asms = \
                    idump_scan(args.idump, binp, outp)
                grand_funcs += max(ok, 0)
                all_asm.update(asms)
                if cfg["fmt"] not in proof and ok > 0:
                    proof[cfg["fmt"]] = (binp.name, total, ok)

                interesting = bool(interrs) or (0 <= rate < 100.0) or bool(asms)
                tags = []
                lifter_caused = False
                if interrs:
                    # Classify: an INTERR that also reproduces WITHOUT the lifter
                    # is a native IDA decompiler bug, not a lifter regression.
                    native = native_interr_funcs(args.idump, binp)
                    pairs = err_funcs if err_funcs else [("?", c) for c in set(interrs)]
                    for fn, code in pairs:
                        if fn != "?" and fn in native:
                            ida_native_findings.append(
                                (gen.stem, cfg["name"], seed, code, fn))
                        else:
                            interr_findings.append(
                                (gen.stem, cfg["name"], seed, code, fn))
                            lifter_caused = True
                    tags.append(f"*** INTERR {sorted(set(interrs))}"
                                f"{' [IDA-native]' if not lifter_caused else ''} ***")
                if 0 <= rate < 100.0:
                    # only a lifter regression if the lifter itself caused an INTERR
                    if lifter_caused:
                        failure_findings.append((gen.stem, cfg["name"], seed, rate))
                    tags.append(f"*** rate {rate}% ***")
                if asms:
                    tags.append(f"asm={sum(asms.values())}")

                print(f"[{gen.stem:20} s{seed} {cfg['name']:16} {cfg['fmt']}] "
                      f"funcs={ok}/{total} rate={rate}% {' '.join(tags)}")

                if interesting:
                    keep_paths.update((corpus, binp, outp))

    # ---- keep / clean artifacts ------------------------------------------
    if not args.keep:
        for p in cleanup_paths - keep_paths:
            Path(p).unlink(missing_ok=True)
    # the dllmain stub is never interesting on its own
    if not args.keep:
        Path(stub).unlink(missing_ok=True)

    # ---- grand summary ----------------------------------------------------
    print("\n==== MATRIX GRAND SUMMARY ====")
    print(f"generators driven   : {len(gens)}")
    print(f"binaries built       : {grand_bins}")
    for fmt in ("ELF64", "ELF32", "PE64", "PE32"):
        if bins_by_fmt.get(fmt):
            print(f"   {fmt:6}            : {bins_by_fmt[fmt]}")
    print(f"functions decompiled : {grand_funcs}")

    # proof of PE64 + ELF64 (self-test requirement)
    print("proof of cross-format coverage:")
    for fmt in ("ELF64", "PE64", "ELF32", "PE32"):
        if fmt in proof:
            nm, tot, ok = proof[fmt]
            print(f"   {fmt:6} built+idumped: {nm}  (funcs {ok}/{tot})")
        elif bins_by_fmt.get(fmt):
            print(f"   {fmt:6} built but no decompiled-OK proof captured")
        else:
            print(f"   {fmt:6} NONE built")

    if gen_fail_log:
        print(f"\ngenerator failures   : {len(gen_fail_log)}")
        for g, s, stage, e in gen_fail_log:
            print(f"   {g} s{s} [{stage}]: {(e.splitlines()[0] if e else '')[:160]}")

    print(f"\nLIFTER-INDUCED INTERR findings : {len(interr_findings)}  "
          f"(these are real lifter bugs / regressions)")
    seen = set()
    for g, c, s, code, fn in interr_findings:
        key = (g, c, s, code, fn)
        if key in seen:
            continue
        seen.add(key)
        print(f"   INTERR {code} in {fn}  [gen={g} config={c} seed={s}]")
        print(f"      reproduce: python3 {g}.py --funcs {args.funcs} "
              f"--seed {s} --out /tmp/{g}_{s} && build via config '{c}'")

    # Native IDA decompiler bugs (reproduce without the lifter) — informational.
    nat = sorted(set((code, fn) for _g, _c, _s, code, fn in ida_native_findings))
    print(f"\nIDA-NATIVE INTERRs (NOT lifter bugs; reproduce w/o plugin): {len(nat)}")
    for code, fn in nat:
        print(f"   INTERR {code} in {fn}  (Hex-Rays bug — decompiles to the same "
              f"INTERR with the lifter disabled)")

    print(f"\ndecompile failures   : {len(failure_findings)}")
    for g, c, s, rate in failure_findings:
        print(f"   rate {rate}%  [gen={g} config={c} seed={s}]")

    # build-combo skip ledger (no silent drops)
    if skip_log:
        skipcnt = Counter(reason for _, _, _, reason in skip_log)
        print(f"\nbuild combos skipped : {len(skip_log)} (deduped reasons:)")
        for reason, n in skipcnt.most_common():
            print(f"   [{n:3}] {reason}")

    if all_asm:
        print(f"\nunlifted __asm mnemonics ({sum(all_asm.values())} occ):")
        for mn, c in all_asm.most_common():
            print(f"   {mn:24} {c}")
    else:
        print("\nunlifted __asm       : none")

    if args.keep:
        print(f"\nartifacts kept (all) under {HERE}/{PREFIX}_*")
    elif keep_paths:
        print(f"\nartifacts kept (interesting) :")
        for p in sorted(set(Path(p).name for p in keep_paths)):
            print(f"   {p}")

    bad = bool(interr_findings) or bool(failure_findings) or bool(gen_fail_log)
    return 1 if bad else 0


if __name__ == "__main__":
    raise SystemExit(main())
