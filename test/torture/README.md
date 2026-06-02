# AVX Lifter Torture Suite

A large, automated, **deterministic** fuzz/torture harness that hunts for lifter
INTERRs (Hex-Rays internal errors), decompilation failures, and instructions
left as raw `__asm`. It exercises every AVX/AVX2/AVX-512/AVX10 family the lifter
supports, in randomized orders and permutations, at all widths, with masked
forms, control flow that keeps vectors/masks live across basic blocks, high
register pressure, memory operands, and cross-function register/state reuse —
to the fringes of what's legal.

## Run it

```bash
make torture                     # ELF64 sweep: 10 seeds x 400 funcs (C + raw-asm)
make matrix                      # FULL wild-conditions matrix (see below) — this
                                 #   is what reproduces the wild INTERRs
make matrix SEEDS=5 FUNCS=400    # crank it up
make seed SEED=7 FUNCS=800       # one reproducible ELF64 seed; keeps _t7.* artifacts
make coverage                    # coverage-closure gate (see below)
```

## Coverage-closure gate (`make coverage` / `coverage_closure.py`)

The matrix finds bugs only in instruction forms the corpus actually emits. An
entire *supported* family can sit untested — that is exactly how the INTERR-50920
memory-operand bugs (vpinsrw, vpclmulqdq, the scalar fp16/cvt family) hid: no
generator folded memory as a reg-or-mem **source**, so those handler paths never
ran. `coverage_closure.py` makes that impossible to repeat:

- The lifter exports an authoritative manifest of every itype it dispatches
  (`avx_match_itype_core()` + k-reg + compare-to-mask), via env `AVX_COV_MANIFEST`.
- It records, per itype, whether the corpus ever fed it a memory **source**
  operand (env `AVX_COV`).
- The gate runs every generator, unions the observed coverage, and **fails** if
  any supported itype with a memory-source form was never exercised with memory,
  unless it is listed in `allowlist_nomem.txt` (instructions with no
  memory-source encoding — `vzeroupper`, `vpextr*`, `vmovmskps`, k-reg ops, etc.
  — each with a justification).

Close a hole by adding corpus coverage (extend `gen_memfold.py`, which folds
memory as a source across widths and sub-dword/scalar sizes) or by allowlisting
it with a reason. `gen_memfold.py` reads `memfold_forms.tsv` (verified, per-itype
folded-memory asm) so its emitted corpus always assembles.

## The wild-conditions matrix (`make matrix` / `torture_matrix.py`)

`torture_matrix.py` drives **every** `gen_*.py` generator across the cross
product of how binaries actually ship in the wild, and idumps each:

- **Compilers/formats:** gcc & clang → ELF64; gcc `-m32` → **ELF32**;
  clang `--target=x86_64-pc-windows-gnu` + lld and `x86_64-w64-mingw32-gcc` →
  **PE64**; `i686-w64-mingw32-gcc` → **PE32**. (PE builds use a `DllMainCRTStartup`
  stub; idump loads PE natively.)
- **Optimization:** `-O0/-O1/-O2/-O3/-Os`.
- **ABI/calling convention:** the ABI corpus tags functions `ms_abi` /
  `sysv_abi` / `vectorcall` / `regcall` and chains calls across them.
- **Encoding fringe & illegal:** EVEX broadcast/rounding/SAE/mask, segment
  overrides, and hand-encoded `.byte` blobs (see generators below).
- 32-bit can't take AVX-512, so the matrix auto-falls-back to each generator's
  `--avx2` mode for ELF32/PE32 (exercises the lifter's 32-bit YMM path); combos
  with no legal build are **logged-skipped, never silently dropped**.
  `CONDITIONS.md` is the exhaustive audit checklist of every axis.

## Generators

- `gen_torture.py` — intrinsic C, all families/widths (also `--avx2` for 32-bit).
- `gen_asm_torture.py` — raw AT&T asm, explicit regs, no save/restore.
- `gen_abi_torture.py` — calling-convention / ABI-boundary stress.
- `gen_evex_fringe.py` — EVEX corner cases: `{1toN}` broadcast, `{r*-sae}`/`{sae}`,
  `{k}{z}`/`{k0}`, zmm16-31, disp8*N, RIP-relative, `fs`/`gs` vector mem, vSIB.
- `gen_illegal.py` — hand-encoded fringe EVEX `.byte` blobs + pathological CFG
  (jumps into mid-instruction, huge bodies, deep nesting, computed jumps).

## Reproduced wild INTERRs

The matrix reproduces real Hex-Rays INTERRs (deterministic per seed):

- **INTERR 50757** — `gen_evex_fringe` functions mixing EVEX broadcast / `{sae}`
  compares / `fs`/`gs`-segment vector memory / gather-scatter. Reproduces under
  both the gcc and clang assemblers (`gcc-asm-elf64`, `clang-asm-elf64`, seed 1).
- **INTERR 50920** — *(FIXED)* `gen_abi_torture` `abi_ms_3` at `-Os` had a
  `vprold zmm, [mem], imm`. The rotate handler read the memory operand's base
  GPR as a 64-byte register (`r12.64`), an oversized read that ran off the GPR
  file into the microcode temporaries (`rt0`/`rt1`) and tripped the
  "temporaries cross block boundaries" check. Fixed by loading the reg-or-mem
  source via `AvxOpLoader` in `handle_v_rotate` / `handle_vpslldq_vpsrldq` /
  `handle_v_shuffle_int`. See `known-issues/lifter-zmm-call-50920/REPORT.md`.

Reproduce a finding (the summary prints the exact command):

```bash
python3 gen_evex_fringe.py --funcs 40 --seed 1 --out e.s
gcc -shared -fPIC $(cat flags.txt) e.s -o e.so
idump --plugin lifter -e e.so          # errors-only: shows the INTERR lines
```

Exits non-zero if any INTERR or decompile failure is found; prints a summary of
INTERR seeds (with kept artifacts to reproduce) and a histogram of any unlifted
`__asm` mnemonics.

## Two corpora

- **`gen_torture.py`** — intrinsic-based C. Guaranteed legal/compilable. Chains
  intrinsics across families with SSA-style dataflow, injects `if`/`for` blocks
  so vector/mask values stay live across blocks (the classic
  "temporaries cross block boundaries" INTERR trigger), threads state through
  globals (cross-function loads/stores), and mixes widths/masking/memory.

- **`gen_asm_torture.py`** — raw AT&T assembly. Explicit registers and **no
  save/restore**, so functions read registers they never wrote (forcing the
  lifter to synthesize `__readzmm`/`__readmask` of cross-function state), use
  high EVEX registers (zmm16-31, k1-7), masked `{k}{z}`, gather/scatter (vSIB +
  mask), compress/expand, and odd orderings. The assembler validates legality.

Both are compiled to shared objects (exported functions → IDA recognizes them)
and dumped with `idump --plugin lifter --pseudo-only`.

## Reproducing a finding

Every failure prints its `seed`. Re-run that one seed with `make seed SEED=<n>`
(artifacts `_t<n>.c` / `_t<n>.s` / `_t<n>.so` / `_t<n>*.out` are kept) and open
the `.out`, or re-dump the `.so` directly:

```bash
idump --plugin lifter --pseudo _t7.so | less
idump --plugin lifter -e _t7_asm.so          # errors-only
```

## Known classes this surfaces

- Masked forms of handlers that only implemented the unmasked path
  (e.g. masked `vsqrtpd`).
- Handlers predating the ZMM modeling that use `reg2mreg` directly: they work
  for zmm0-15 but leave zmm16-31 as `__asm` (e.g. `vpermps`). These are prime
  suspects for wild INTERRs when real code touches the high EVEX registers.
