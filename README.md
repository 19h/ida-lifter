<h1 align="center">lifter</h1>

<img width="1536" height="581" alt="lifter" src="logo-narrow.png" />

---

<h5 align="center">
lifter is a Hex-Rays microcode filter that lifts AVX/AVX2/AVX-512 instructions to Intel intrinsics.<br/>
Where IDA shows opaque <code>__asm</code> blocks, lifter emits readable <code>_mm256_add_ps</code> calls.<br/>
200+ instructions transformed. Zero manual annotation required.<br/>
<br/>
Scalar operations compile to native FP microcode for optimal decompiler output.<br/>
FMA, gather, shuffle, broadcast—all lifted to their documented intrinsic forms.<br/>
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

- **200+ AVX/AVX2 instructions** lifted to readable intrinsics
- **Scalar operations** (`vaddss`, `vmulss`, etc.) use native FP microcode for clean output
- **FMA instructions** including all 132/213/231 forms with memory operands
- **AVX-512 register operations** (ZMM) with partial support
- **Invisible vzeroupper** - no more noise from transition instructions

## Supported Instructions

| Category | Instructions |
|----------|-------------|
| **Arithmetic** | vaddps/pd, vsubps/pd, vmulps/pd, vdivps/pd, vminps/pd, vmaxps/pd |
| **Scalar** | vaddss/sd, vsubss/sd, vmulss/sd, vdivss/sd, vminss/sd, vmaxss/sd |
| **FMA** | vfmadd/vfmsub/vfnmadd/vfnmsub (132/213/231, ps/pd/ss/sd) |
| **Integer** | vpaddb/w/d/q, vpsubb/w/d/q, vpmullw/d, vpmaddwd |
| **Bitwise** | vpand, vpor, vpxor, vpandn, vandps/pd, vorps/pd, vxorps/pd |
| **Shuffle** | vshufps/pd, vpshufd, vpshufb, vpermq, vpermd, vpermilps |
| **Blend** | vblendps/pd, vpblendd, vpblendw, vblendvps/pd |
| **Compare** | vcmpps/pd (all predicates), vpcmpeqb/w/d/q, vpcmpgtb/w/d/q |
| **Broadcast** | vbroadcastss/sd/f128/i128, vpbroadcastb/w/d/q |
| **Gather** | vgatherdps/pd, vpgatherdd/dq |
| **Convert** | vcvtdq2ps, vcvtps2dq, vcvtps2pd, vcvtpd2ps, etc. |
| **Extend** | vpmovsxbw/bd/bq/wd/wq/dq, vpmovzxbw/bd/bq/wd/wq/dq |
| **Move** | vmovaps/pd/dqa/dqu/ups/upd, vmovss/sd, vmovshdup/sldup/ddup |
| **Extract** | vextractf128, vextracti128, vextractps |
| **Insert** | vinsertf128, vinserti128, vpinsrb/w/d/q |
| **Unpack** | vunpckhps/pd, vunpcklps/pd, vpunpckh/lbw/wd/dq/qdq |
| **Special** | vsqrtps/pd/ss/sd, vrsqrtps/ss, vrcpps/ss, vroundps/pd/ss/sd |
| **Mask** | vmovmskps/pd, vpmovmskb |
| **SAD** | vpsadbw, vmpsadbw |

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

The test suite is located in `test/` with CMake-based builds. See `test/README.md` for detailed documentation.

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

Build and run:
```bash
cd test
mkdir build && cd build
cmake ..
make                    # Build all tests
make shooter            # Build shooter game
make decompiler_ref     # Build physics simulations
./decompiler_ref        # Run physics simulations (~40 sec)
```

## Building

**Requirements:** IDA SDK 7.5+ (tested with 9.2), CMake 3.10+, Clang/GCC with C++17

```bash
# Set IDA SDK path
export IDASDK=/path/to/idasdk

# Build and install
make install
```

On macOS, the plugin is automatically code-signed. Without signing, IDA will hang on load.

## Known Limitations

### AVX-512 EVEX Instructions
- **ZMM memory operands**: Fall back to IDA's handling due to SDK limitations
- **Masked operations** (`{k1}`, `{k1}{z}`): Not lifted, shown as `__asm`
- **Compare-to-mask** (`vcmpps k1, ymm0, ymm1`): IDA limitation, causes INTERR 50311

### Hex-Rays Limitations
- **ZMM registers**: "Unsupported processor register 'zmm0'" is a Hex-Rays limitation, not plugin bug
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

**4. Pointer Type Arguments (INTERR 50732)**

Gather instructions need proper pointer types:
```cpp
// Wrong: tinfo_t(BT_PTR) has size 0
// Right:
tinfo_t ptr_type;
ptr_type.create_ptr(tinfo_t(BT_VOID));
```

**5. kreg Lifetime (INTERR 50420)**

Never free kregs that are referenced by emitted instructions - the microcode engine manages their lifetime.

### Verification Approach

1. **Test binaries** in `tests/` with isolated instruction patterns
2. **Real-world binaries** (`test/physics/shooter`) with complex SIMD code
3. **idafn_dump** tool for batch decompilation and error detection
4. **Iterative debugging** using IDA's INTERR codes to pinpoint issues

## Architecture

```
src/avx/
├── avx_lifter.cpp      # Main filter (match/apply dispatch)
├── avx_intrinsic.cpp   # Intrinsic call builder
├── avx_helpers.cpp     # Operand loading, register mapping
├── avx_types.cpp       # Vector type synthesis (__m128, __m256, __m512)
├── avx_utils.cpp       # Instruction classification
└── handlers/
    ├── handler_mov.cpp   # Move instructions
    ├── handler_math.cpp  # Arithmetic, FMA
    ├── handler_logic.cpp # Bitwise, shuffle, blend
    └── handler_cvt.cpp   # Conversions, extends
```

## Error Reference

| INTERR | Cause | Fix |
|--------|-------|-----|
| 50420 | Freeing kreg still in use | Don't free kregs used in emitted instructions |
| 50732 | Invalid pointer type | Use `create_ptr(BT_VOID)` not `BT_PTR` |
| 50757 | Operand size > 8 bytes | Set UDT flag on large operands |
| 50801 | FP flag on integer opcode | Use m_fadd not m_add for floats |
| 50920 | Size mismatch across blocks | Zero-extend scalars to match type size |

## Not Lifted (Fall Back to IDA)

These instructions are explicitly not handled and use IDA's default behavior:
- **vptest** - Flag-setting only, no register destination
- **vcomiss/vucomiss** - Scalar compares (converted to SSE via `try_convert_to_sse()`)
- **Mask manipulation** - kandw, korw, knotw, etc. (k-register primary operands)
- **Embedded broadcast** - `{1to16}`, `{1to8}` memory broadcast
- **Rounding override** - `{rn-sae}`, `{ru-sae}`, etc.

## Debug Mode

Enable verbose logging to trace instruction matching:

```cpp
// In avx_debug.cpp
set_debug_logging(true);
```

Output:
```
[AVXLifter::DEBUG] 100007D16: MATCH itype=830
[AVXLifter::DEBUG] 100007D16: >>> ENTER apply
```

## License

MIT

## See Also

- `CLAUDE.md` - Detailed technical documentation with implementation notes
- `test/README.md` - Complete test suite documentation
- `test/unit/` - Individual instruction tests
- `test/integration/` - Multi-instruction complex tests
- `test/physics/` - Real-world physics simulation test suite
