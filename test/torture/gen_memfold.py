#!/usr/bin/env python3
"""Folded-memory-source torture generator (gas/AT&T asm, ELF64).

WHY THIS EXISTS
---------------
The other generators only ever produce memory as a *standalone, full-width*
load bound to a register variable; they never fold a memory operand as the
reg-or-mem SOURCE of a compute instruction. That blind spot hid a whole bug
class in the lifter: when an instruction's reg-or-mem source is MEMORY, the
handler must load it (and at the right size) instead of reading the base GPR
as a vector / reading past the loaded bytes. Both failure modes raise
Hex-Rays INTERR 50920 ("temporaries cross block boundaries") -- but ONLY when
the bad/undefined temp is live across a basic-block edge.

So every function here is a tight LOOP that:
  * folds a memory operand as an instruction's source (incl. sub-dword/scalar
    sizes that no other generator emits: m8/m16/m32/m64), and
  * carries a vector accumulator across the loop back-edge and stores it,
which is exactly the control-flow shape that turns an over-read's undefined
sub-range into a live-in temp at the edge.

Covers xmm/ymm/zmm widths and the families that were historically broken:
inserts, pclmul/aes/gf, immediate shuffles/rotates/byte-shifts, 3-operand
logic/arith, 3-operand-with-imm (shuf/ternlog/align), and scalar fp/convert.

CLI contract (matched by torture_matrix.py): --funcs N --seed S --out FILE
Optional: --avx2 restricts to AVX/AVX2 xmm/ymm forms (for ELF32/PE32 builds).

Usage: gen_memfold.py --funcs 400 --seed 1 --out memfold.s
"""
import argparse
import random

# Calling convention (SysV): rdi = const* src, rsi = void* out, edx = count.
# Each template returns the loop-body instruction(s). The harness wraps it in a
# counted loop carrying the accumulator vector register `V<w>0`.

def vreg(width):
    return {16: "%xmm0", 32: "%ymm0", 64: "%zmm0"}[width]

def vstore(width):
    # store the accumulator so it stays live across the back-edge
    return {16: "vmovdqu64 %xmm0, (%rsi)",
            32: "vmovdqu64 %ymm0, (%rsi)",
            64: "vmovdqu64 %zmm0, (%rsi)"}[width]


# Each entry: (tag, requires_avx512, body_lines_fn(rng)).
# body uses (%rdi) as the folded memory source and the accumulator vreg.
def build_templates():
    T = []

    def add(tag, av512, fn):
        T.append((tag, av512, fn))

    # --- scalar GPR-source inserts: sub-dword memory (the original bug shape) ---
    add("vpinsrb", False, lambda r: [f"vpinsrb ${r.randint(0,15)}, (%rdi), %xmm0, %xmm0", vstore(16)])
    add("vpinsrw", False, lambda r: [f"vpinsrw ${r.randint(0,7)}, (%rdi), %xmm0, %xmm0", vstore(16)])
    add("vpinsrd", False, lambda r: [f"vpinsrd ${r.randint(0,3)}, (%rdi), %xmm0, %xmm0", vstore(16)])
    add("vpinsrq", False, lambda r: [f"vpinsrq ${r.randint(0,1)}, (%rdi), %xmm0, %xmm0", vstore(16)])

    # --- carry-less multiply / AES / GFNI with folded memory (xmm + ymm) ---
    add("vpclmulqdq.x", False, lambda r: [f"vpclmulqdq ${r.choice([0,1,16,17])}, (%rdi), %xmm0, %xmm0", vstore(16)])
    add("vpclmulqdq.y", False, lambda r: [f"vpclmulqdq ${r.choice([0,1,16,17])}, (%rdi), %ymm0, %ymm0", vstore(32)])
    add("vaesenc.x",    False, lambda r: ["vaesenc (%rdi), %xmm0, %xmm0", vstore(16)])
    add("vaesenc.y",    False, lambda r: ["vaesenc (%rdi), %ymm0, %ymm0", vstore(32)])
    add("vgf2p8affineqb.x", False, lambda r: [f"vgf2p8affineqb ${r.randint(0,255)}, (%rdi), %xmm0, %xmm0", vstore(16)])
    add("vgf2p8affineqb.y", False, lambda r: [f"vgf2p8affineqb ${r.randint(0,255)}, (%rdi), %ymm0, %ymm0", vstore(32)])

    # --- immediate single-source forms (shuffle / rotate / byte-shift) ---
    add("vpshufd.x", False, lambda r: [f"vpshufd ${r.randint(0,255)}, (%rdi), %xmm0", vstore(16)])
    add("vpshufd.y", False, lambda r: [f"vpshufd ${r.randint(0,255)}, (%rdi), %ymm0", vstore(32)])
    add("vpshufd.z", True,  lambda r: [f"vpshufd ${r.randint(0,255)}, (%rdi), %zmm0", vstore(64)])
    add("vpslldq.x", False, lambda r: [f"vpslldq ${r.randint(0,15)}, (%rdi), %xmm0", vstore(16)])
    add("vprold.z",  True,  lambda r: [f"vprold ${r.randint(0,31)}, (%rdi), %zmm0", vstore(64)])
    add("vpermilps.y", False, lambda r: [f"vpermilps ${r.randint(0,255)}, (%rdi), %ymm0", vstore(32)])

    # --- 3-operand logic/arith with src1 = accumulator, folded memory src2 ---
    add("vpand.x",  False, lambda r: ["vpand (%rdi), %xmm0, %xmm0", vstore(16)])
    add("vpaddd.y", False, lambda r: ["vpaddd (%rdi), %ymm0, %ymm0", vstore(32)])
    add("vmulps.z", True,  lambda r: ["vmulps (%rdi), %zmm0, %zmm0", vstore(64)])
    add("vpxorq.z", True,  lambda r: ["vpxorq (%rdi), %zmm0, %zmm0", vstore(64)])

    # --- 3-operand-with-imm, folded memory ---
    add("vshufps.y",   False, lambda r: [f"vshufps ${r.randint(0,255)}, (%rdi), %ymm0, %ymm0", vstore(32)])
    add("vpternlogd.z", True, lambda r: [f"vpternlogd ${r.randint(0,255)}, (%rdi), %zmm0, %zmm0", vstore(64)])
    add("valignd.z",    True, lambda r: [f"valignd ${r.randint(0,15)}, (%rdi), %zmm0, %zmm0", vstore(64)])

    # --- scalar fp with folded memory (m32 / m64) ---
    add("vaddss",  False, lambda r: ["vaddss (%rdi), %xmm0, %xmm0", "vmovss %xmm0, (%rsi)"])
    add("vmulsd",  False, lambda r: ["vmulsd (%rdi), %xmm0, %xmm0", "vmovsd %xmm0, (%rsi)"])
    add("vsqrtsd", False, lambda r: ["vsqrtsd (%rdi), %xmm0, %xmm0", "vmovsd %xmm0, (%rsi)"])
    add("vcvtsi2sd", False, lambda r: ["vcvtsi2sdl (%rdi), %xmm0, %xmm0", "vmovsd %xmm0, (%rsi)"])

    # --- scalar FP16 with folded m16 memory (the comish / cvt-sh family) ---
    add("vcvtsh2sd", True, lambda r: ["vcvtsh2sd (%rdi), %xmm0, %xmm0", "vmovsd %xmm0, (%rsi)"])
    add("vaddsh",    True, lambda r: ["vaddsh (%rdi), %xmm0, %xmm0", "vmovsh %xmm0, (%rsi)"])
    add("vcomish",   True, lambda r: ["vcomish (%rdi), %xmm0", "vmovsh %xmm0, (%rsi)"])
    add("vcvtsh2si", True, lambda r: ["vcvtsh2si (%rdi), %ecx", "mov %ecx, (%rsi)"])

    return T


def emit_func(name, body_lines, init_width):
    lab = f".L_{name}"
    out = [f".globl {name}", f".type {name}, @function", f"{name}:"]
    # zero the accumulator we use (xmm0/ymm0/zmm0 alias) and the counter
    out.append("    vpxor %xmm0, %xmm0, %xmm0")
    out.append("    xor %eax, %eax")
    out.append(f"{lab}:")
    for ins in body_lines:
        out.append(f"    {ins}")
    out.append("    inc %eax")
    out.append("    cmp %eax, %edx")
    out.append(f"    jg {lab}")
    out.append("    ret")
    out.append(f".size {name}, .-{name}")
    return out


import os


def load_forms_file(path):
    """Read verified memory-folded forms: TSV lines `tag <TAB> body`, where body
    is ';'-separated gas/AT&T loop-body lines using (%rdi)=mem src, %xmm0/%ymm0/
    %zmm0=accumulator, (%rsi)=store. Produced by coverage triage; each line is
    pre-assembled so gen_memfold output stays buildable. Returns [(tag, fn)]."""
    forms = []
    if not path or not os.path.exists(path):
        return forms
    for line in open(path):
        line = line.rstrip("\n")
        if not line or line.startswith("#") or "\t" not in line:
            continue
        tag, body = line.split("\t", 1)
        body_lines = [b.strip() for b in body.split(";") if b.strip()]
        forms.append((tag.strip(), (lambda bl: (lambda r: list(bl)))(body_lines)))
    return forms


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--funcs", type=int, default=400)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--out", default="memfold.s")
    ap.add_argument("--avx2", action="store_true",
                    help="restrict to AVX/AVX2 xmm/ymm forms (no AVX-512)")
    ap.add_argument("--forms", default=None,
                    help="extra verified forms TSV (default: memfold_forms.tsv "
                         "beside this script, if present)")
    args = ap.parse_args()

    rng = random.Random(args.seed * 1000003 + 17)
    templates = build_templates()
    if args.avx2:
        templates = [t for t in templates if not t[1]]
    # Append verified data-driven forms (av512=False so they survive --avx2 too;
    # forms that need AVX-512 simply won't assemble on 32-bit and are skipped).
    forms_path = args.forms or os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                            "memfold_forms.tsv")
    if not args.avx2:
        templates = templates + [(tag, False, fn) for tag, fn in load_forms_file(forms_path)]

    lines = [
        "# auto-generated folded-memory-source torture (gas/AT&T, ELF64)",
        f"# seed={args.seed} funcs={args.funcs} avx2={args.avx2}",
        ".text",
    ]
    # Emit at least one function per template so EVERY form is exercised
    # regardless of --funcs (the coverage gate depends on this), then fill up
    # to --funcs by cycling.
    total = max(args.funcs, len(templates))
    for i in range(total):
        tag, _av512, fn = templates[i % len(templates)]
        body = fn(rng)
        name = f"mf_{tag.replace('.', '_')}_{i}"
        lines += emit_func(name, body, 16)
        lines.append("")

    with open(args.out, "w") as f:
        f.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
