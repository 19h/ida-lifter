<h1 align="center">lifter</h1>

<img width="1536" height="581" alt="lifter" src="logo-narrow.png" />

---

<h5 align="center">
 lifter is a Hex-Rays microcode filter that lifts AVX/AVX2/AVX-512/AVX10 instructions to Intel intrinsics.<br/>
 Where IDA shows opaque <code>__asm</code> blocks, lifter emits readable <code>_mm512_mask_add_ps</code> calls.<br/>
 Hundreds of instruction forms transformed. Zero manual annotation required.<br/>
<br/>
 Scalar operations compile to native FP microcode for optimal decompiler output.<br/>
 Masked EVEX (merge/zero), FP16/BF16, gather/scatter, and permute ops lift to their documented intrinsic forms.<br/>
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
- **FP16/BF16/IFMA/VNNI coverage** including scalar FP16 (sh) and complex FP16 (fcmul/fmadd)
- **ZMM memory operands + vector stores** supported via UDT-flagged loads and store intrinsics
- **Scalar ops** use native FP microcode; **vzeroupper** becomes a NOP
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
| **Shuffle/permute** | vpshufb/d, vpermq/vpermd, vpermilps/pd, vpermb/vpermw, vpermt2* |
| **Blend/pack/unpack** | vblend*, vpunpck*, vpack*, vpmovsx/zx, vpmovwb, vpmovdb/dw/qb/qd |
| **Compare** | vcmpps/pd/ss/sd, vpcmpeqb/w/d/q, vpcmpgtb/w/d/q |
| **Broadcast** | vbroadcastss/sd/f128/i128, vbroadcastf32x4/f64x4, vpbroadcastb/w/d/q |
| **Move/extract/insert** | vmovaps/upd/dqa/dqu, vmovss/sd, vmovsh/vmovw, vmovnt*, vmaskmov*, vmovshdup/sldup/ddup, vextractf128/i128, vinsertf128/i128, vextractps/vinsertps |
| **Convert** | vcvtdq2ps, vcvtps2dq, vcvtps2pd, vcvtpd2ps, vcvt{ps/pd}{u}qq, vcvtph<->(ps/pd/w/uw) |
| **Approx/round** | vrcp*/vrsqrt*, vrcp14*/vrsqrt14*, vround*/vrndscale*, vgetexp*, vgetmant*, vfixupimm*, vscalef*, vrange*, vreduce* |
| **Gather/scatter** | vgatherd*/q*, vpgather*, vscatterd*/q*, vpscatter* |
| **Compress/expand** | vcompressps/pd, vexpandps/pd, vpcompressb/w/d/q, vpexpandb/w/d/q |
| **Mask/misc** | vmovmskps/pd, vpmovmskb, vpmovm2b/w/d/q, vpsadbw/vmpsadbw, vpopcnt*, vplzcnt*, vpconflict*, vzeroupper |
| **Crypto** | GFNI, AES, VPCLMULQDQ, SHA1/SHA256 |
| **Cache control** | clflushopt, clwb |

Most instruction families above also lift AVX-512VL masked merge/zero forms; opmask operands appear as immediates (k1 -> 1) because Hex-Rays doesn't expose k-registers in microcode.

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

The primary test suite lives in `test/` (CMake + Makefile wrapper). AVX-512/AVX10 coverage and missing-asm reporting live in `avx10/`. Legacy object-based regressions remain in `tests/` and `suite/`. See `test/README.md` for detailed documentation.

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

- `avx10/` - Comprehensive AVX-512/AVX10 test binary + `avx10_missing_asm_latest.txt` report
- `tests/` and `suite/` - legacy object-based regression artifacts

Build and run (Makefile wrapper):
```bash
cd test
make
make shooter
make decompiler_ref
./build/decompiler_ref
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

**Requirements:** IDA SDK 7.5+ (tested with 9.2), CMake 3.10+, Ninja (for the Makefile path), Clang/GCC with C++17

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
- **Opmask destinations** (compare-to-mask, kmov/kunpck) are emitted as NOPs; mask state is not modeled
- **Mask values** are passed as immediates (k1 -> 1, k2 -> 2) because Hex-Rays doesn't expose k-registers
- **EVEX features** like embedded broadcast `{1to16}/{1to8}`, rounding override `{rn-sae}`, and mask2 forms still fall back to IDA
- **Unlisted masked ops** still fall back to IDA (only the families listed above are lifted with masking)

### 32-bit Mode
- **YMM operations** are not lifted in 32-bit binaries; XMM-only lifting is supported

### Hex-Rays Limitations
- **ZMM registers**: "Unsupported processor register 'zmm0'" is a Hex-Rays limitation, not plugin bug. Expect warnings during `idump` runs; functions still decompile.
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

### Verification Approach

1. **Unit/integration tests** in `test/` with isolated instruction patterns
2. **AVX-512/AVX10 coverage** in `avx10/` with missing-asm reports
3. **Real-world binaries** (`test/physics/shooter`) with complex SIMD code
4. **idafn_dump** tool for batch decompilation and error detection
5. **Iterative debugging** using IDA's INTERR codes to pinpoint issues

## Architecture

```
src/
├── plugin/
│   ├── lifter_plugin.cpp      # Plugin entry + popup integration
│   └── component_registry.cpp # Component registration
├── avx/
│   ├── avx_lifter.cpp      # Main filter (match/apply dispatch)
│   ├── avx_intrinsic.cpp   # Intrinsic call builder
│   ├── avx_helpers.cpp     # Operand loading, masking, store helpers
│   ├── avx_types.cpp       # Vector type synthesis (__m128, __m256, __m512)
│   ├── avx_utils.cpp       # Instruction classification
│   ├── avx_debug.cpp       # Disassembly/microcode debug output
│   └── handlers/
│       ├── handler_mov.cpp   # Move, gather/scatter, compress/expand
│       ├── handler_math.cpp  # Arithmetic, FMA, FP16/BF16/IFMA/VNNI
│       ├── handler_logic.cpp # Bitwise, shifts, permutes, masks
│       └── handler_cvt.cpp   # Conversions, extends
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
| 50757 | Operand size > 8 bytes | Set UDT flag on large operands |
| 50801 | FP flag on integer opcode | Use m_fadd not m_add for floats |
| 50920 | Size mismatch across blocks | Zero-extend scalars to match type size |

## Not Lifted (Fall Back to IDA)

These instructions are explicitly not handled and use IDA's default behavior:
- **vptest** - Flag-setting only, no register destination
- **vcomiss/vucomiss/vcvttss2si/vcvttsd2si** - converted to SSE via `try_convert_to_sse()` and left to IDA
- **k-register destinations** (compare-to-mask, kmov/kunpck) - emitted as NOPs to avoid INTERR 50311
- **Mask-only ops** (kand/kor/kxor/knot, etc.) - not lifted
- **Embedded broadcast** - `{1to16}`, `{1to8}` memory broadcast
- **Rounding override** - `{rn-sae}`, `{ru-sae}`, etc.
- **EVEX mask2 forms** - fall back to IDA

## Debug Mode

Enable verbose logging to trace instruction matching:

```python
import idaapi
idaapi.set_debug_logging(True)
```

Logging is enabled by default at plugin init; call `idaapi.set_debug_logging(False)` to quiet it.

Optional disassembly/microcode dumps are implemented in `src/avx/avx_debug.cpp`, but the Hex-Rays callback is currently disabled in `src/avx/avx_lifter.cpp` (commented out). Re-enable the callback and rebuild to use it (see `QUICK_START.md` and `DEBUG_PRINTING.md`).

Output:
```
[AVXLifter::DEBUG] 100007D16: MATCH itype=830
[AVXLifter::DEBUG] 100007D16: >>> ENTER apply
```

## License

MIT

## See Also

- `test/README.md` - Complete test suite documentation
