#!/usr/bin/env python3
"""Coverage-closure gate for the AVX lifter.

PURPOSE
-------
Make it impossible for a *supported* instruction family to sit untested -- the
exact failure that hid the INTERR-50920 memory-operand bugs (vpinsrw, vpclmulqdq,
the scalar fp16/cvt family). The lifter dispatches ~860 itypes; the torture
corpus historically exercised most of them only in register form and never fed
whole families a memory operand.

HOW
---
1. The lifter exports an authoritative manifest of every itype it dispatches
   (env AVX_COV_MANIFEST -> avx_match_itype_core() + k-reg + compare-to-mask).
2. It records, per itype, whether the corpus ever fed it a memory *source*
   operand (env AVX_COV, bit1 = seen-with-src-mem).
3. This script runs every generator across seeds, unions the observed coverage,
   and FAILS if any supported itype with a memory-source form was never
   exercised with memory -- unless it is in allowlist_nomem.txt (instructions
   that genuinely have no memory-source encoding, each with a reason).

A supported itype is a hole when it is either:
  * NEVER seen at all (no generator emits it), or
  * seen only in register form (never with a memory source),
and it is not allowlisted. Closing a hole = add corpus coverage (extend a
generator, usually gen_memfold.py) OR allowlist it with a justification.

Usage:
    coverage_closure.py --seeds 2 --funcs 200 --idump idump
    coverage_closure.py --report-only          # don't fail, just print holes
"""
import argparse
import glob
import os
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
PREFIX = "_cov"
FLAGS = (HERE / "flags.txt").read_text().strip()
ALLOWLIST = HERE / "allowlist_nomem.txt"

# Generators that produce *supported* AVX instructions (gen_illegal is about
# illegal/garbage handling and is intentionally excluded from coverage).
GEN_GLOB = "gen_*.py"
GEN_SKIP = {"gen_illegal.py"}


def sh(cmd, env=None, timeout=900):
    return subprocess.run(cmd, shell=True, text=True, capture_output=True,
                          timeout=timeout, env=env)


def detect_kind(path: Path) -> str:
    head = path.read_text(errors="replace")[:4096]
    if "#include" in head or "__m512" in head:
        return "c"
    if head.lstrip().startswith(".") or ".globl" in head or ".text" in head:
        return "asm"
    return "c" if path.suffix == ".c" else "asm"


def build(src: Path, kind: str) -> Path | None:
    so = src.with_suffix(".so")
    # asm and C both build with gcc + the project feature flags (ELF64).
    r = sh(f"gcc -shared -fPIC {FLAGS} {src} -o {so} 2>&1")
    if r.returncode != 0 or not so.exists():
        return None
    return so


def load_allowlist() -> dict:
    """itype-mnemonic -> reason. Lines: '<mnem>  # reason'. '#' lines ignored."""
    allow = {}
    if not ALLOWLIST.exists():
        return allow
    for line in ALLOWLIST.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        if "#" in line:
            mnem, reason = line.split("#", 1)
        else:
            mnem, reason = line, ""
        mnem = mnem.strip()
        if mnem:
            allow[mnem] = reason.strip()
    return allow


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--seeds", type=int, default=2)
    ap.add_argument("--funcs", type=int, default=200)
    ap.add_argument("--idump", default="idump")
    ap.add_argument("--report-only", action="store_true",
                    help="print holes but exit 0 (for triage)")
    ap.add_argument("--keep", action="store_true")
    args = ap.parse_args()

    cov_file = HERE / f"{PREFIX}_observed.tsv"
    manifest_file = HERE / f"{PREFIX}_manifest.tsv"
    for f in (cov_file, manifest_file):
        f.unlink(missing_ok=True)

    gens = [Path(g) for g in sorted(glob.glob(str(HERE / GEN_GLOB)))
            if Path(g).name not in GEN_SKIP]
    print(f"[cov] generators: {', '.join(g.name for g in gens)}")
    print(f"[cov] seeds={args.seeds} funcs={args.funcs}")

    built = 0
    for gen in gens:
        for seed in range(1, args.seeds + 1):
            base = HERE / f"{PREFIX}_{gen.stem}_s{seed}"
            out_c = base.with_suffix(".c")
            r = sh(f"python3 {gen} --funcs {args.funcs} --seed {seed} --out {out_c}")
            if r.returncode != 0 or not out_c.exists():
                print(f"[cov]   skip {gen.name} s{seed}: generate failed")
                continue
            kind = detect_kind(out_c)
            src = out_c
            if kind == "asm":
                src = base.with_suffix(".s")
                os.replace(out_c, src)
            so = build(src, kind)
            if so is None:
                print(f"[cov]   skip {gen.name} s{seed}: build failed")
                continue
            env = dict(os.environ)
            env["AVX_COV"] = str(cov_file)
            # dump the manifest exactly once (first successful idump)
            if not manifest_file.exists():
                env["AVX_COV_MANIFEST"] = str(manifest_file)
            sh(f"{args.idump} --plugin lifter --pseudo-only --no-color {so} >/dev/null 2>&1",
               env=env)
            built += 1
            # tidy IDA db files
            for ext in (".id0", ".id1", ".id2", ".nam", ".til", ".i64"):
                Path(str(so) + ext).unlink(missing_ok=True)

    if not manifest_file.exists():
        print("[cov] ERROR: no manifest produced (no successful idump). Is the "
              "plugin installed and AVX_COV_MANIFEST honored?", file=sys.stderr)
        return 2

    # --- aggregate ---
    supported = {}  # mnem -> itype
    for line in manifest_file.read_text().splitlines():
        itype, mnem = line.split("\t")
        supported[mnem] = int(itype)

    seen = {}        # mnem -> unioned flag bits
    for line in cov_file.read_text().splitlines():
        itype, mnem, bits = line.split("\t")
        seen[mnem] = seen.get(mnem, 0) | int(bits)

    allow = load_allowlist()

    never_seen = sorted(m for m in supported if m not in seen)
    reg_only = sorted(m for m in supported
                      if seen.get(m, 0) & 1 and not (seen.get(m, 0) & 2))

    holes_never = [m for m in never_seen if m not in allow]
    holes_regonly = [m for m in reg_only if m not in allow]

    print(f"\n[cov] {built} corpora idumped")
    print(f"[cov] supported itypes : {len(supported)}")
    print(f"[cov] seen (any form)  : {len(seen)}")
    print(f"[cov] seen w/ src-mem  : {sum(1 for v in seen.values() if v & 2)}")
    print(f"[cov] allowlisted      : {len(allow)}")
    print("-" * 70)
    print(f"[cov] HOLE: supported but NEVER generated ({len(holes_never)}):")
    print("        " + " ".join(holes_never) if holes_never else "        (none)")
    print(f"[cov] HOLE: generated but NEVER with a memory source ({len(holes_regonly)}):")
    print("        " + " ".join(holes_regonly) if holes_regonly else "        (none)")

    if not args.keep:
        for f in glob.glob(str(HERE / f"{PREFIX}_*")):
            if not f.endswith(".tsv"):
                Path(f).unlink(missing_ok=True)

    total_holes = len(holes_never) + len(holes_regonly)
    if total_holes and not args.report_only:
        print(f"\n[cov] FAIL: {total_holes} coverage hole(s). Either add corpus "
              f"coverage (extend gen_memfold.py) or allowlist with a reason in "
              f"{ALLOWLIST.name}.")
        return 1
    print(f"\n[cov] {'OK (report-only)' if args.report_only else 'OK: coverage closed'}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
