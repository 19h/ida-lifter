<h1 align="center">lifter</h1>

<img width="1536" height="581" alt="lifter" src="logo-narrow.png" />

---

<h5 align="center">
 lifter is a Hex-Rays microcode filter that lifts AVX/AVX2/AVX-512/AVX10 and VMX/VT-x instructions to intrinsics.<br/>
 Where IDA shows opaque <code>__asm</code> blocks, lifter emits readable <code>_mm512_mask_add_ps</code> and <code>__vmread</code> calls.<br/>
 Hundreds of instruction forms transformed. Zero manual annotation required.<br/>
<br/>
 Scalar operations compile to native FP microcode for optimal decompiler output.<br/>
 Masked EVEX (merge/zero), FP16/BF16, gather/scatter, and permute ops lift to their documented intrinsic forms.<br/>
 Opmask (k-register) compares, logic, and mask&harr;vector moves are modeled via <code>__readmask</code>/<code>__writemask</code>; ZMM state via <code>__readzmm</code>/<code>__writezmm</code>.<br/>
 VMX virtualization instructions become readable function calls.<br/>
The SIMD becomes legible. The algorithms become obvious.
</h5>

## Before & After

**Without plugin:**
```c
__asm { vmovups ymm0, ymmword ptr [rdi] }
__asm { vaddps ymm0, ymm0, ymmword ptr [rsi] }
__asm { vmovups ymmword ptr [rdx], ymm0 }
```

**With plugin:**
```c
*(__m256 *)rdx = _mm256_add_ps(*(__m256 *)rdi, *(__m256 *)rsi);
```

## Features

- **Hundreds of AVX/AVX2/AVX-512/AVX10 forms** lifted to Intel intrinsics (XMM/YMM/ZMM + AVX-512VL)
- **Masked EVEX ops** (merge/zero) across arithmetic, bitwise, permute/shuffle, compare, gather/scatter, compress/expand
- **Opmask (k-register) modeling**: compare-into-mask, `vfpclass`/`vptestm`, mask&harr;vector (`vpmovm2*`/`vpmov*2m`), and the k-ALU (`kand`/`kor`/`kxor`/`knot`/`kshift`/`kunpck`/`kmov`) read/write k-state via `__readmask`/`__writemask` helpers
- **ZMM register modeling**: direct ZMM register operands (including zmm16-31) and cross-function ZMM state are modeled via `__readzmm`/`__writezmm`; ZMM memory operands + vector stores use UDT-flagged loads and store intrinsics
- **FP16/BF16/IFMA/VNNI coverage** including scalar FP16 (sh) and complex FP16 (fcmul/fmadd)
- **Scalar ops** use native FP microcode; **vzeroupper** becomes a NOP
- **VMX/VT-x instructions** lifted to intrinsic-style calls (`__vmxon`, `__vmread`, `__vmwrite`, etc.)
- **Inline/outlining toggle** actions in Hex-Rays pseudocode context menus

## Supported Instructions

| Category | Instructions (examples) |
|----------|-------------------------|
| **Packed FP** | vaddps/pd, vsubps/pd, vmulps/pd, vdivps/pd, vminps/pd, vmaxps/pd, vaddsubps/pd |
| **Scalar FP** | vaddss/sd, vsubss/sd, vmulss/sd, vdivss/sd, vminss/sd, vmaxss/sd, vsqrtss/sd |
| **FMA** | vfmadd/vfmsub/vfnmadd/vfnmsub (132/213/231, ps/pd/ss/sd), vfmaddsub/vfmsubadd |
| **FP16** | vaddph/vsubph/vmulph/vdivph, vaddsh/vmulsh/vsqrtsh, vfmadd*ph/sh, vfcmul/fcmadd |
| **Integer math** | vpadd*, vpsub*, vpmull*, vpmaddwd/ubsw, vpavg*, vpabs*, vpsign* |
| **Bitwise + ternary** | vpand/or/xor, vandps/pd, vpternlogd/q |
| **Shifts/rotates** | vpsll*/vpsrl*/vpsra*, vpsllv*/vpsrlv*, vprol*/vpror*, vpshld*/vpshrd*, vpmultishiftqb |
| **Shuffle/permute** | vpshufb/d, vshufps/pd, vpermps/vpermq/vpermd, vpermilps/pd, vpermb/vpermw, vpermt2*, 128-bit lane shuffles (vshuff32x4/i32x4, vshuff64x2/i64x2) |
| **Blend/pack/unpack** | vblend*, vpunpck*, vpack*, vpmovsx/zx, vpmovwb, vpmovdb/dw/qb/qd |
| **Compare** | vcmpps/pd/ss/sd, vpcmpeqb/w/d/q, vpcmpgtb/w/d/q |
| **Into-mask (k dest)** | vcmpps/pd/ss/sd, vpcmp{u}b/w/d/q, vptestm/vptestnm*, vfpclassps/pd/ss/sd/ph/sh &rarr; `..._mask` intrinsics written through `__writemask` |
| **Opmask (k-regs)** | kand/kandn/kor/kxor/kxnor/knot, kadd, kshiftl/kshiftr, kunpck, kmov, mask&harr;vector (vpmovm2b/w/d/q, vpmovb/w/d/q2m) |
| **Broadcast** | vbroadcastss/sd/f128/i128, vbroadcastf32x4/f64x4, vpbroadcastb/w/d/q (XMM/mem and GPR source) |
| **Move/extract/insert** | vmovaps/upd/dqa/dqu, vmovss/sd, vmovsh/vmovw, vmovhps/lps/hpd/lpd, vmovlhps/vmovhlps, vmovnt*, vmaskmov*, vmovshdup/sldup/ddup, vextractf128/i128, vinsertf128/i128, vextractps/vinsertps, vldmxcsr/vstmxcsr |
| **Convert** | vcvtdq2ps, vcvtps2dq, vcvtps2pd, vcvtpd2ps, vcvt{ps/pd}{u}qq, vcvtph<->(ps/pd/w/uw) |
| **Approx/round** | vrcp*/vrsqrt*, vrcp14*/vrsqrt14*, vround*/vrndscale*, vgetexp*, vgetmant*, vfixupimm*, vscalef*, vrange*, vreduce* |
| **Gather/scatter** | vgatherd*/q*, vpgather*, vscatterd*/q*, vpscatter* |
| **Compress/expand** | vcompressps/pd, vexpandps/pd, vpcompressb/w/d/q, vpexpandb/w/d/q |
| **Test/mask/misc** | vptest/vtestps/pd, vmovmskps/pd, vpmovmskb, vpsadbw/vmpsadbw, vpopcnt*, vplzcnt*, vpconflict*, vzeroupper |
| **Crypto** | GFNI, AES, VPCLMULQDQ, SHA1/SHA256 |
| **Cache control** | clflushopt, clwb |

Most instruction families above also lift AVX-512VL masked merge/zero forms. The writemask *predicate* `{k}` on a masked data op is passed as an **immediate** (k1 -> 1), since the mask selector isn't a true microcode value. Mask *operands and results* of the mask-centric ops (the **Into-mask** and **Opmask** rows above) are modeled with `__readmask(idx)` / `__writemask(idx, value)` helper calls so the k-register dataflow is visible.

## VMX/VT-x Instructions

The VMX lifter component handles Intel virtualization instructions:

| Instruction | Lifted To | Description |
|-------------|-----------|-------------|
| `vmxon [mem]` | `__vmxon(ptr)` | Enter VMX operation |
| `vmxoff` | `__vmxoff()` | Leave VMX operation |
| `vmcall` | `__vmcall()` | Call VM monitor |
| `vmlaunch` | `__vmlaunch()` | Launch virtual machine |
| `vmresume` | `__vmresume()` | Resume virtual machine |
| `vmptrld [mem]` | `__vmptrld(ptr)` | Load VMCS pointer |
| `vmptrst [mem]` | `__vmptrst(ptr)` | Store VMCS pointer |
| `vmclear [mem]` | `__vmclear(ptr)` | Clear VMCS |
| `vmread dst, enc` | `__vmread(&dst, enc)` | Read VMCS field |
| `vmwrite enc, src` | `__vmwrite(enc, src)` | Write VMCS field |
| `invept type, m128` | `__invept(type, desc)` | Invalidate EPT entries |
| `invvpid type, m128` | `__invvpid(type, desc)` | Invalidate VPID entries |
| `vmfunc` | `__vmfunc()` | VM function call |

**VMX Before & After:**

```c
// Without plugin:
__asm { vmxon   [rbp+var_10] }
__asm { vmread  [rbp+var_10], rdi }
__asm { vmwrite rdi, rsi }

// With plugin:
__vmxon(&v4);
__vmread(&v5, encoding);
__vmwrite(encoding, value);
```

## Example Output

**Fluid simulation diffuse step** (`test/physics/src/fluid.c`):

```c
// Decompiled with plugin - readable vector operations
v7 = _mm256_set1_ps((float)(a5.f32[0] * a5.f32[0]) * 1860.0);
v8 = _mm256_set1_ps(1.0 / _mm_fmadd_ss(v6, (__m128)0x45E88000u, a6).f32[0]);
for ( j = 0; j != 7680; j += 256 ) {
    *(__m256 *)(v9 + j - 448) = _mm256_mul_ps(
        _mm256_fmadd_ps(
            v7,
            _mm256_add_ps(
                _mm256_add_ps(
                    _mm256_add_ps(*(__m256 *)(v9 + j - 444), *(__m256 *)(v9 + j - 452)),
                    *(__m256 *)(v9 + j - 704)),
                *(__m256 *)(v9 + j - 192)),
            *(__m256 *)(a2 + j + 260)),
        v8);
}
```

## Test Suite

The maintained test suite lives in `test/` (CMake + Makefile wrapper). Experimental AVX-512/AVX10 coverage and missing-asm reports live in `test/experimental/avx10/`. See `test/README.md` for detailed documentation.

```
test/
├── CMakeLists.txt              # Unified build system
├── unit/                       # ~75 individual instruction tests
│   ├── sources/               # Test implementations (test_vaddps.c, etc.)
│   └── stubs/                 # Test harnesses by signature
├── integration/               # Multi-instruction complex tests
│   ├── avx_comprehensive_test.c  # Exhaustive coverage (~2000 lines)
│   ├── test_fluid_advect.c       # Gather, round, FMA, blend
│   └── test_scalar.c             # Scalar AVX operations
├── torture/                   # Deterministic INTERR/__asm fuzz harness
│   ├── gen_*.py                  # Generators (intrinsic, raw-asm, ABI, EVEX fringe, illegal)
│   ├── torture_matrix.py         # Wild-conditions matrix across compilers/formats/opt levels
│   └── known-issues/             # Reproduced INTERR reports
└── physics/                   # Real-world SIMD workloads
    ├── decompiler_ref         # Physics simulation testbed (4 sims, ~40 sec)
    ├── shooter                # Primary validation binary (~48 functions)
    └── src/
        ├── nbody.c            # N-body gravity (AVX-512)
        ├── waves.c            # Wave interference (AVX2)
        ├── particles.c        # Particle swarm (AVX)
        ├── fluid.c            # Navier-Stokes fluid vortex (AVX2)
        └── bullet.c           # 2D shooter with enemy AI, A* pathfinding (AVX)
```

- `test/torture/` - Deterministic fuzz/torture harness that hunts for lifter INTERRs and unlifted `__asm` across every AVX family, at all widths, with masked forms and cross-block live values (see `test/torture/README.md`)
- `test/experimental/avx10/` - Explicit-only AVX-512/AVX10 object corpus + missing-asm reports
- `tests/` - legacy object-based regression artifacts

Torture suite (finds INTERRs and `__asm` gaps; exits non-zero on any failure):
```bash
make -C test torture                 # ELF64 sweep (intrinsic + raw-asm)
make -C test torture_matrix          # full wild-conditions matrix (reproduces real INTERRs)
make -C test/torture seed SEED=7     # reproduce one ELF64 seed, keeps _t7.* artifacts
```

Build and run (Makefile wrapper):
```bash
make test              # build the plugin test suite under test/
make -C test unit_tests
make -C test test_vpermps
make -C test test_vmovlhps
make -C test run_comprehensive
make test-avx10        # optional; requires AVX10-capable Clang
```

Decompiler validation requires IDA and the installed plugin:
```bash
make install
idump --plugin lifter --pseudo test/build/test_vpermps
idump --plugin lifter --pseudo test/build/test_vmovlhps
```

Manual CMake:
```bash
cd test
mkdir build && cd build
cmake ..
make
```

Optional: `test/Makefile` includes `shooter-wasm` targets for WebAssembly builds via Docker + Emscripten.

## Building

**Requirements:** IDA SDK 7.5+ (tested with 9.2–9.3.1), CMake 3.10+, Ninja (for the Makefile path), Clang/GCC with C++17

The build auto-detects both the legacy library layout (`x64_linux_gcc_64`) and the modern layout without the compiler name (`x64_linux_64`) introduced in IDA SDK 9.3.1+.

```bash
# Set IDA SDK path (or use ~/ida-sdk or ~/idasdk91)
export IDASDK=/path/to/idasdk

# Build and install (CMake + Ninja)
make build
make install
```

If you prefer a different generator, run CMake directly and skip the Makefile wrapper.

On macOS, the plugin is automatically code-signed. Without signing, IDA will hang on load.

## Known Limitations

### AVX-512/AVX10 EVEX Instructions
- **Opmask modeling is helper-based, not a true Hex-Rays def-use chain.** k0-k7 aren't microcode-addressable, so `__readmask`/`__writemask` (and `__readzmm`/`__writezmm`) calls stand in for the register state. In realistic chains (a mask feeds a later k-op) this holds; in a trivial function that only *returns* a freshly-computed mask, DCE can drop the `__writemask` because the mask→return link is opaque.
- **Writemask predicate** on a masked data op is passed as an **immediate** (k1 -> 1, k2 -> 2) — the `{k}` selector isn't a real microcode value.
- **`kortest`/`ktest`** set EFLAGS only; the flag effect isn't modeled, so they are still emitted as NOPs. `vcomish`/`vucomish` are best-effort.
- **Older handlers predating ZMM modeling** (e.g. `vpermps`, `vsqrtpd`) use `reg2mreg` directly: they lift correctly for zmm0-15 but leave **zmm16-31** operands as `__asm`.
- **EVEX features** like embedded broadcast `{1to16}/{1to8}`, rounding override `{rn-sae}`, and mask2 forms still fall back to IDA.
- **fs/gs segment-override** vector memory operands decline to IDA `__asm` (avoids a Hex-Rays microcode-gen INTERR; see below).
- **Unlisted masked ops** still fall back to IDA (only the families listed above are lifted with masking).

### 32-bit Mode
- **YMM operations** are not lifted in 32-bit binaries; XMM-only lifting is supported

### Hex-Rays Limitations
- **ZMM registers**: "Unsupported processor register 'zmm0'" is a Hex-Rays limitation, not a plugin bug. Because the lifter virtualizes ZMM state through helper calls, these `WARN_UNSUPP_REG` warnings are now stripped from the decompiler output (`suppress_lifter_warnings` in `src/plugin/lifter_plugin.cpp`); functions decompile cleanly.
- **YMM/XMM aliasing**: Function signatures may show `__m128` when `__m256` is expected

### Third-Party Plugin Conflicts
The **goomba** MBA oracle plugin can cause false decompilation failures:
```
[!] Decompilation failed at 0:FFFFFFFFFFFFFFFF: emulator: unknown operand type
```
**Fix:** Disable goomba's auto mode.

## Development Journey

### The Challenge
IDA's Hex-Rays decompiler shows many AVX instructions as `__asm` blocks, making SIMD code unreadable. We built a microcode filter that intercepts these instructions and generates proper intrinsic calls.

### Key Technical Hurdles

**1. Operand Size Mismatches (INTERR 50920)**

Intel intrinsics like `_mm_min_ss(__m128 a, __m128 b)` expect 128-bit types even for scalar ops. When loading a 4-byte scalar from memory, we must zero-extend to 16 bytes:
```cpp
// Load 4-byte scalar, extend to XMM for intrinsic
mreg_t t = mba->alloc_kreg(XMM_SIZE);
cdg.emit(m_xdu, &src_4byte, nullptr, &dst_16byte);
```

**2. FMA Instruction Detection Bug**

The IDA SDK enum groups FMA by data type, not form:
```
NN_vfmadd132pd, NN_vfmadd132ps, NN_vfmadd132sd, NN_vfmadd132ss,  // +0,+1,+2,+3
NN_vfmadd213pd, NN_vfmadd213ps, ...                               // +4,+5,...
```
We initially assumed +1 stride between 132/213/231 forms. The actual stride is +4.

**3. AVX-512 UDT Flag (INTERR 50757)**

IDA's verifier rejects operand sizes > 8 bytes unless marked as UDT (User Defined Type):
```cpp
if (size > 8) {
    call_insn->d.set_udt();
    mov_insn->l.set_udt();
    mov_insn->d.set_udt();
}
```

**4. ZMM Memory Operands + Vector Stores (INTERR 50757/50708)**

Large loads/stores must set UDT before verification and avoid raw m_stx stores:
```cpp
// ZMM load: manual m_ldx with UDT
mreg_t dst = cdg.mba->alloc_kreg(ZMM_SIZE, false);
mop_t d(dst, ZMM_SIZE);
d.set_udt();
cdg.emit(m_ldx, &seg, &off, &d);

// Vector store: use store intrinsic
AVXIntrinsic icall(&cdg, "_mm512_storeu_ps");
icall.add_argument_reg(addr, ptr_type);
icall.add_argument_reg(src, vec_type);
icall.emit_void();
```

**5. Opmask Register Encoding**

Hex-Rays microcode does not model k-registers, so `_mm*_mask*` calls receive the mask as an immediate (k1 -> 1).

**6. Pointer Type Arguments (INTERR 50732)**

Gather instructions need proper pointer types:
```cpp
// Wrong: tinfo_t(BT_PTR) has size 0
// Right:
tinfo_t ptr_type;
ptr_type.create_ptr(tinfo_t(BT_VOID));
```

**7. kreg Lifetime (INTERR 50420)**

Never free kregs that are referenced by emitted instructions - the microcode engine manages their lifetime.

**8. Opmask & ZMM Register Modeling**

Hex-Rays microcode can't address k0-k7 or zmm registers directly. Rather than emit NOPs and lose the dataflow, the lifter models them with helper calls: `__readmask(idx)` / `__writemask(idx, value)` for opmask registers, mirroring `__readzmm` / `__writezmm` for ZMM state. Compare-into-mask, `vfpclass`/`vptestm`, mask&harr;vector moves, and the k-ALU all route through these, so a mask produced by one instruction and consumed by another stays visible. The tradeoff: the helpers don't form a true def-use chain, so DCE can drop a `__writemask` whose only consumer is an opaque return (see Known Limitations).

**9. Torture-Discovered INTERRs (50757 segment override, 50920 oversized GPR read)**

The `test/torture/` matrix surfaced two real Hex-Rays INTERRs, both lifter-induced:
- **50757** — a vector instruction with an `fs`/`gs` segment-override memory operand crashed microcode generation. Fix: `match()` declines when `insn.segpref == R_fs || R_gs`, so IDA renders `__asm` instead.
- **50920** ("temporaries cross block boundaries") — the imm-form rotate/shift/shuffle handlers read a memory operand's *base GPR* as a 64-byte register (`r12.64`). That oversized read runs off the GPR file into microcode temps (rt0/rt1), which then appear as undefined live-ins. Fix: load reg-or-mem sources via `AvxOpLoader` in `handle_v_rotate` / `handle_vpslldq_vpsrldq` / `handle_v_shuffle_int`.

### Verification Approach

1. **Unit/integration tests** in `test/` with isolated instruction patterns
2. **Torture suite** in `test/torture/` — deterministic fuzz matrix across compilers/formats/opt levels/ABIs that hunts for INTERRs and unlifted `__asm`
3. **AVX-512/AVX10 coverage** in `test/experimental/avx10/` with missing-asm reports
4. **Real-world binaries** (`test/physics/shooter`) with complex SIMD code
5. **idafn_dump** tool for batch decompilation and error detection
6. **Iterative debugging** using IDA's INTERR codes (and `AVX_DUMP_MC=1` microcode dumps) to pinpoint issues

## Architecture

```
src/
├── plugin/
│   ├── lifter_plugin.cpp      # Plugin entry + popup integration + ZMM warning suppression
│   └── component_registry.cpp # Component registration
├── avx/
│   ├── avx_lifter.cpp      # AVX microcode filter (match/apply dispatch)
│   ├── avx_intrinsic.cpp   # Intrinsic call builder
│   ├── avx_helpers.cpp     # Operand loading, store helpers, opmask/ZMM modeling (__readmask/__writemask, __readzmm/__writezmm)
│   ├── avx_types.cpp       # Vector type synthesis (__m128, __m256, __m512)
│   ├── avx_utils.cpp       # Instruction classification
│   ├── avx_debug.cpp       # Disassembly/microcode debug output
│   └── handlers/
│       ├── handler_mov.cpp   # Move, gather/scatter, compress/expand
│       ├── handler_math.cpp  # Arithmetic, FMA, FP16/BF16/IFMA/VNNI
│       ├── handler_logic.cpp # Bitwise, shifts, permutes, masks
│       └── handler_cvt.cpp   # Conversions, extends
├── vmx/
│   └── vmx_lifter.cpp      # VMX/VT-x microcode filter
├── inline/
│   └── inline_component.cpp  # Inline/outlining actions in pseudocode
└── common/
    ├── warn_off.h
    └── warn_on.h
```

## Error Reference

| INTERR | Cause | Fix |
|--------|-------|-----|
| 50311 | Compare-to-mask/k-reg dest | IDA limitation; lift as NOP |
| 50420 | Freeing kreg still in use | Don't free kregs used in emitted instructions |
| 50708 | Vector store emission failure | Use store intrinsics for large stores |
| 50732 | Invalid pointer type | Use `create_ptr(BT_VOID)` not `BT_PTR` |
| 50757 | Operand size > 8 bytes; or fs/gs segment-override vector mem | Set UDT flag on large operands; decline segment-override forms in `match()` |
| 50801 | FP flag on integer opcode | Use m_fadd not m_add for floats |
| 50920 | Undefined live-in temporary crossing blocks (scalar size mismatch, or oversized GPR read off the register file) | Zero-extend scalars to match type size; load reg-or-mem sources via `AvxOpLoader` instead of `reg2mreg` on a mem operand's base GPR |

### Lifter QASSERT IDs (0xAxxxx)

These are internal lifter assertion IDs used in `QASSERT(...)`. If you see an `INTERR` or assert with one of these values, it usually means a handler precondition failed. Decimal values are included because IDA often reports errors in decimal.

| Error (hex/dec) | File | Description |
|---------------|------|-------------|
| 0xA0300 (656128) | `src/avx/handlers/handler_mov.cpp` | handle_vmov_ss_sd: expects a memory source in Op2 (scalar load form). |
| 0xA0301 (656129) | `src/avx/handlers/handler_mov.cpp` | handle_vmov_ss_sd: expects a memory destination in Op1 and XMM source in Op2 (scalar store form). |
| 0xA0302 (656130) | `src/avx/handlers/handler_mov.cpp` | handle_vmov_ss_sd: expects Op1/Op2/Op3 to be XMM registers (three-operand form). |
| 0xA0303 (656131) | `src/avx/handlers/handler_mov.cpp` | handle_vmov: expects Op2 to be an XMM register (register source). |
| 0xA0306 (656134) | `src/avx/handlers/handler_mov.cpp` | handle_vmov_sh: expects Op1/Op2 to be XMM registers (scalar FP16 move). |
| 0xA0310 (656144) | `src/avx/handlers/handler_mov.cpp` | handle_v_mov_ps_dq: expects a memory source in Op2 (mem→reg path). |
| 0xA0400 (656384) | `src/avx/handlers/handler_logic.cpp` | handle_v_bitwise: expects Op1/Op2 to be vector registers. |
| 0xA0401 (656385) | `src/avx/handlers/handler_logic.cpp` | handle_v_bitwise: unexpected opcode reached default switch case. |
| 0xA0402 (656386) | `src/avx/handlers/handler_logic.cpp` | handle_v_rotate: expects an immediate control in Op3. |
| 0xA0410 (656400) | `src/avx/handlers/handler_logic.cpp` | handle_v_shift_double: expects an immediate control in Op4. |
| 0xA0500 (656640) | `src/avx/handlers/handler_math.cpp` | handle_v_math_ss_sd: expects Op1/Op2 to be AVX registers (scalar FP math). |
| 0xA0501 (656641) | `src/avx/handlers/handler_math.cpp` | handle_v_math_p: expects Op1/Op2 to be vector registers (packed math). |
| 0xA0502 (656642) | `src/avx/handlers/handler_math.cpp` | handle_v_math_p: unexpected opcode reached default switch case. |
| 0xA0503 (656643) | `src/avx/handlers/handler_math.cpp` | handle_v_minmax_ss_sd: expects Op1/Op2 to be XMM registers (scalar min/max). |
| 0xA0504 (656644) | `src/avx/handlers/handler_math.cpp` | handle_v_hmath: expects Op1/Op2 to be vector registers (horizontal math). |
| 0xA0505 (656645) | `src/avx/handlers/handler_math.cpp` | handle_v_dot: expects Op1/Op2 to be vector registers (dot product). |
| 0xA0506 (656646) | `src/avx/handlers/handler_math.cpp` | handle_v_dot: expects an immediate control in Op4. |
| 0xA0507 (656647) | `src/avx/handlers/handler_math.cpp` | handle_vround: expects an immediate control in Op3. |
| 0xA0600 (656896) | `src/avx/handlers/handler_math.cpp` | handle_vsqrtss: expects Op1/Op2 to be XMM registers (scalar sqrt). |
| 0xA0601 (656897) | `src/avx/handlers/handler_logic.cpp` | handle_vshufps: expects an immediate control in Op4. |
| 0xA0602 (656898) | `src/avx/handlers/handler_logic.cpp` | handle_vshufpd: expects an immediate control in Op4. |
| 0xA0603 (656899) | `src/avx/handlers/handler_logic.cpp` | handle_vpermpd: expects an immediate control in Op3. |
| 0xA0604 (656900) | `src/avx/handlers/handler_logic.cpp` | handle_vblend_imm_ps_pd: expects an immediate control in Op4. |
| 0xA0605 (656901) | `src/avx/handlers/handler_logic.cpp` | handle_v_shuffle_int: expects an immediate control in Op3. |
| 0xA0607 (656903) | `src/avx/handlers/handler_logic.cpp` | handle_v_align: expects an immediate control in Op4. |
| 0xA0608 (656904) | `src/avx/handlers/handler_logic.cpp` | handle_v_ternary_logic: expects an immediate control in Op4. |
| 0xA0608 (656904) | `src/avx/handlers/handler_math.cpp` | handle_vrcp_rsqrt_ss: expects Op1/Op2 to be XMM registers (scalar rcp/rsqrt). |
| 0xA0609 (656905) | `src/avx/handlers/handler_math.cpp` | handle_vround_ss_sd: expects Op1/Op2 to be XMM registers (scalar round). |
| 0xA060B (656907) | `src/avx/handlers/handler_math.cpp` | handle_vsqrt_sh: expects Op1/Op2 to be XMM registers (scalar FP16 sqrt). |
| 0xA0610 (656912) | `src/avx/handlers/handler_logic.cpp` | handle_vblend_int: expects an immediate control in Op4. |
| 0xA0610 (656912) | `src/avx/handlers/handler_math.cpp` | handle_vround_ss_sd: expects an immediate control in Op4. |
| 0xA0611 (656913) | `src/avx/handlers/handler_math.cpp` | handle_vsqrtsd: expects Op1/Op2 to be XMM registers (scalar sqrt double). |
| 0xA0620 (656928) | `src/avx/handlers/handler_math.cpp` | handle_v_getmant: expects an immediate control in Op3. |
| 0xA0620 (656928) | `src/avx/handlers/handler_math.cpp` | handle_v_getmant: expects an immediate control in Op4. |
| 0xA0621 (656929) | `src/avx/handlers/handler_math.cpp` | handle_v_fixupimm: expects an immediate control in Op4. |
| 0xA0622 (656930) | `src/avx/handlers/handler_math.cpp` | handle_v_range: expects an immediate control in Op4. |
| 0xA0623 (656931) | `src/avx/handlers/handler_math.cpp` | handle_v_reduce: expects an immediate control in Op3. |
| 0xA0623 (656931) | `src/avx/handlers/handler_math.cpp` | handle_v_reduce: expects an immediate control in Op4. |
| 0xA0700 (657152) | `src/avx/handlers/handler_logic.cpp` | handle_vperm2f128_i128: expects Op1/Op2 to be YMM registers. |
| 0xA0700 (657152) | `src/avx/handlers/handler_math.cpp` | handle_v_math_ph: expects Op1/Op2 to be vector registers (packed FP16 math). |
| 0xA0701 (657153) | `src/avx/handlers/handler_logic.cpp` | handle_vbroadcastf128_fp: expects YMM destination in Op1 and memory source in Op2. |
| 0xA0701 (657153) | `src/avx/handlers/handler_logic.cpp` | handle_vperm2f128_i128: expects an immediate control in Op4. |
| 0xA0701 (657153) | `src/avx/handlers/handler_math.cpp` | handle_v_math_sh: expects Op1/Op2 to be XMM registers (scalar FP16 math). |
| 0xA0703 (657155) | `src/avx/handlers/handler_logic.cpp` | handle_vbroadcasti128_int: expects YMM destination in Op1 and memory source in Op2. |
| 0xA0704 (657156) | `src/avx/handlers/handler_logic.cpp` | handle_vbroadcast_x4: expects a vector destination in Op1. |
| 0xA0800 (657408) | `src/avx/handlers/handler_logic.cpp` | handle_vmaskmov_ps_pd: expects a memory operand in Op3 (load form). |
| 0xA0801 (657409) | `src/avx/handlers/handler_logic.cpp` | handle_vmaskmov_ps_pd: expects memory destination in Op1 and AVX reg sources in Op2/Op3 (store form). |
| 0xA0900 (657664) | `src/avx/handlers/handler_logic.cpp` | handle_vextractf128: expects an immediate control in Op3. |
| 0xA0902 (657666) | `src/avx/handlers/handler_logic.cpp` | handle_vextractf128: expects a memory destination in Op1. |
| 0xA0910 (657680) | `src/avx/handlers/handler_logic.cpp` | handle_vinsertf128: expects an immediate control in Op4. |
| 0xA0A00 (657920) | `src/avx/handlers/handler_logic.cpp` | handle_vmovnt: expects a memory source in Op2 (vmovntdqa load). |
| 0xA0A01 (657921) | `src/avx/handlers/handler_logic.cpp` | handle_vmovnt: expects a memory destination in Op1 (non-temporal store). |
| 0xA0A02 (657922) | `src/avx/handlers/handler_logic.cpp` | handle_v_mask_to_vec: expects Op2 to be a k-register or encoded k-reg (o_kreg/o_reg). |
| 0xA0A10 (657936) | `src/avx/handlers/handler_logic.cpp` | handle_vpinsert: expects an immediate control in Op4. |
| 0xA0A20 (657952) | `src/avx/handlers/handler_logic.cpp` | handle_vpslldq_vpsrldq: expects an immediate shift count in Op3. |
| 0xA0A30 (657968) | `src/avx/handlers/handler_logic.cpp` | handle_v_gfni: expects an immediate control in Op4. |
| 0xA0A30 (657968) | `src/avx/handlers/handler_logic.cpp` | handle_vextractps: expects an immediate control in Op3. |
| 0xA0A31 (657969) | `src/avx/handlers/handler_logic.cpp` | handle_v_pclmul: expects an immediate control in Op4. |
| 0xA0A32 (657970) | `src/avx/handlers/handler_logic.cpp` | handle_v_sha: expects Op1 to be an XMM register. |
| 0xA0A33 (657971) | `src/avx/handlers/handler_logic.cpp` | handle_v_sha: expects an immediate control in Op3 (sha1rnds4 imm8). |
| 0xA0A40 (657984) | `src/avx/handlers/handler_logic.cpp` | handle_vinsertps: expects an immediate control in Op4. |

## Not Lifted (Fall Back to IDA)

These instructions are explicitly not handled and use IDA's default behavior:
- **vcomiss/vucomiss/vcvttss2si/vcvttsd2si** - converted to SSE via `try_convert_to_sse()` and left to IDA
- **kortest/ktest** - EFLAGS-only; emitted as NOPs (flag effect not modeled)
- **Embedded broadcast** - `{1to16}`, `{1to8}` memory broadcast
- **Rounding override** - `{rn-sae}`, `{ru-sae}`, etc.
- **EVEX mask2 forms** - fall back to IDA
- **fs/gs segment-override vector memory operands** - declined in `match()` (avoids INTERR 50757)

> Previously listed here but **now lifted**: `vptest`/`vtestps`/`vtestpd` (→ `__vptest*`/`__vtest_*`), compare-into-mask and other k-register destinations (→ `__writemask`), and the mask-only k-ALU (`kand`/`kor`/`kxor`/`knot`/`kshift`/`kunpck`/`kmov`).

## Debug Mode

Enable verbose logging to trace instruction matching:

```python
import idaapi
idaapi.set_debug_logging(True)
```

Logging is enabled by default at plugin init; call `idaapi.set_debug_logging(False)` to quiet it.

To diagnose verifier INTERRs, set the `AVX_DUMP_MC` environment variable before launching IDA/idump. When set, the lifter installs a Hex-Rays callback that dumps the full microcode at generation time and again at the moment of any internal error (with block use/def lists), so the offending construct — e.g. an undefined live-in temporary behind INTERR 50920 — is visible:

```bash
AVX_DUMP_MC=1 idump --plugin lifter --pseudo test/build/test_vpermps
```

Output:
```
[AVXLifter::DEBUG] 100007D16: MATCH itype=830
[AVXLifter::DEBUG] 100007D16: >>> ENTER apply
```

## License

MIT

## See Also

- `test/README.md` - Complete test suite documentation
