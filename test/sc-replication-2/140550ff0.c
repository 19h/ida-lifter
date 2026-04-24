#include <stdint.h>
#include <immintrin.h>
#include <string.h>

// Prevent optimization with volatile
#define FORCE_USE(x) __asm__ volatile("" :: "r,m"(x) : "memory")
#define FORCE_READ(x) ({ __typeof__(x) __tmp; __asm__ volatile("" : "=r,m"(__tmp) : "0,m"(x)); __tmp; })

// Forward declarations
typedef struct TaskContext TaskContext;
typedef struct SourceData SourceData;
typedef struct ParamBlock ParamBlock;

// 0xE0-byte source structure (from rdx)
typedef struct __attribute__((aligned(32))) SourceData {
    uint8_t bytes[0xE0];
} SourceData;

// VTable for inner object at [rbx+88h]
typedef struct InnerVTable {
    uint64_t pad[8];
    void* (*method_40)(void* self, void* param);  // offset 0x40
} InnerVTable;

// Inner object
typedef struct InnerObject {
    InnerVTable* vtable;
} InnerObject;

// VTable for main object
typedef struct MainVTable {
    uint64_t pad[17];
    int64_t (*method_88)(void* self, void* data, void* r8_param);  // offset 0x88
} MainVTable;

// Main object structure (rcx/rbx) - must match offsets
typedef struct __attribute__((packed)) TaskContext {
    MainVTable* vtable;         // offset 0x00
    uint8_t pad_08[0x30];       // offset 0x08
    uint8_t field_38[0x10];     // offset 0x38 - xmm1 source
    uint64_t field_40;          // offset 0x48 -> but disasm says 0x40!
    uint8_t pad_50[0x38];       // padding
    InnerObject* inner_obj;     // offset 0x88
} TaskContext;

// Correct struct with proper offsets
typedef struct TaskContextFixed {
    MainVTable* vtable;         // 0x00
    uint8_t pad1[0x38];         // 0x08-0x3F
    uint8_t xmm1_src[0x10];     // 0x38 (overlaps - use direct offset)
    uint64_t field_40;          // 0x40
    uint8_t pad2[0x40];         // 0x48-0x87
    InnerObject* inner_obj;     // 0x88
} TaskContextFixed;

// Stack frame structure matching var_168
typedef struct ParamBlock {
    uint64_t qword_00;          // 0x00
    uint8_t xmm1_at_08[0x10];   // 0x08
    uint8_t xmm0_at_18[0x10];   // 0x18
} ParamBlock;

// External symbol
extern void Subsumption__Task__InitializeTask(void* rcx, void* rdx, void* r8);

// Force noinline to preserve structure
__attribute__((noinline, ms_abi))
int64_t sub_140550FF0(void* rcx_arg, void* rdx_arg, void* r8_arg)
{
    // Stack frame: 0x180 bytes
    // Map original var locations:
    // var_E8 = rsp + 0xA0  (0x188 - 0xE8 = 0xA0) - 0xE0 byte copy
    // var_168 = rsp + 0x20 (0x188 - 0x168 = 0x20) - ParamBlock
    // var_140 = rsp + 0x48
    // var_138 = rsp + 0x50
    // var_128 = rsp + 0x60
    // var_108 = rsp + 0x80
    // var_F8 = rsp + 0x90
    // var_90 = rsp + 0xF8
    // var_80 = rsp + 0x108
    
    volatile uint8_t stack_frame[0x180] __attribute__((aligned(32)));
    
    // Pointers into stack frame
    volatile uint8_t* var_E8 = &stack_frame[0xA0];   // 0xE0 byte copy destination
    volatile uint8_t* var_168 = &stack_frame[0x20]; // ParamBlock
    volatile uint32_t* var_140 = (uint32_t*)&stack_frame[0x48];
    volatile uint64_t* var_138 = (uint64_t*)&stack_frame[0x50];
    volatile uint8_t* var_128 = &stack_frame[0x60];
    volatile uint8_t* var_108 = &stack_frame[0x80];
    volatile uint64_t* var_F8 = (uint64_t*)&stack_frame[0x90];
    volatile uint64_t* var_90 = (uint64_t*)&stack_frame[0xF8];
    volatile uint8_t* var_80 = &stack_frame[0x108];
    volatile uint8_t* var_70 = &stack_frame[0x118];
    volatile uint8_t* var_C8 = &stack_frame[0xC0];
    
    // Preserve rbx, rdi as in original
    void* volatile rbx = rcx_arg;
    void* volatile rdi = r8_arg;
    uint8_t* rdx = (uint8_t*)rdx_arg;
    uint8_t* rcx = (uint8_t*)rcx_arg;
    
    // ===== COPY 0xE0 BYTES FROM RDX TO STACK =====
    
    // vmovups ymm0, [rdx]
    __m256 ymm0 = _mm256_loadu_ps((float*)(rdx + 0x00));
    // vmovups ymm1, [rdx+80h]  
    __m256 ymm1 = _mm256_loadu_ps((float*)(rdx + 0x80));
    
    // vmovups [rax], ymm0  (rax = &var_E8)
    _mm256_storeu_ps((float*)(var_E8 + 0x00), ymm0);
    
    // vmovups ymm0, [rdx+20h]
    ymm0 = _mm256_loadu_ps((float*)(rdx + 0x20));
    // vmovups [rax+20h], ymm0
    _mm256_storeu_ps((float*)(var_E8 + 0x20), ymm0);
    
    // vmovups ymm0, [rdx+40h]
    ymm0 = _mm256_loadu_ps((float*)(rdx + 0x40));
    // vmovups [rax+40h], ymm0
    _mm256_storeu_ps((float*)(var_E8 + 0x40), ymm0);
    
    // vmovups xmm0, [rdx+60h]
    __m128 xmm0_f = _mm_loadu_ps((float*)(rdx + 0x60));
    // vmovups [rax+60h], xmm0
    _mm_storeu_ps((float*)(var_E8 + 0x60), xmm0_f);
    
    // vmovups xmm0, [rdx+70h]
    xmm0_f = _mm_loadu_ps((float*)(rdx + 0x70));
    // vmovups [rax+70h], xmm0 (after lea rax, [rax+80h]; vmovups [rax-10h], xmm0)
    _mm_storeu_ps((float*)(var_E8 + 0x70), xmm0_f);
    
    // vmovups [rax], ymm1 (rax now at var_E8+0x80)
    _mm256_storeu_ps((float*)(var_E8 + 0x80), ymm1);
    
    // vmovups ymm1, [rdx+0A0h]
    ymm1 = _mm256_loadu_ps((float*)(rdx + 0xA0));
    // vmovups [rax+20h], ymm1
    _mm256_storeu_ps((float*)(var_E8 + 0xA0), ymm1);
    
    // vmovups ymm1, [rdx+0C0h]
    ymm1 = _mm256_loadu_ps((float*)(rdx + 0xC0));
    // vmovups [rax+40h], ymm1
    _mm256_storeu_ps((float*)(var_E8 + 0xC0), ymm1);
    
    // ===== XOR CHAIN COMPUTATION =====
    
    // vmovups xmm1, [rcx+38h]
    __m128i xmm1 = _mm_loadu_si128((__m128i*)(rcx + 0x38));
    
    // vmovups xmm0, [r11-80h]  (var_80 in copied data = var_E8 + 0x68)
    __m128i xmm0 = _mm_loadu_si128((__m128i*)(var_E8 + 0x68));
    
    // mov ecx, [r11-70h]  (var_70 = var_E8 + 0x78)
    uint32_t ecx_val = *(volatile uint32_t*)(var_E8 + 0x78);
    
    // mov rax, qword ptr [rsp+188h+var_E8] = first qword of copied data
    uint64_t rax_val = *(volatile uint64_t*)var_E8;
    
    // mov edx, ecx
    uint64_t rdx_hash = ecx_val;
    
    // mov r8, [r11-0C8h]  (var_C8 = var_E8 + 0x20)
    uint64_t r8_val = *(volatile uint64_t*)(var_E8 + 0x20);
    FORCE_USE(r8_val);
    
    // mov [rsp+188h+var_140], ecx
    *var_140 = ecx_val;
    
    // vmovq rcx, xmm0
    uint64_t rcx_extract = (uint64_t)_mm_cvtsi128_si64(xmm0);
    
    // xor rdx, rcx
    rdx_hash ^= rcx_extract;
    
    // Store to var_168+0 (qword ptr [rsp+188h+var_168])
    *(volatile uint64_t*)(var_168 + 0x00) = rax_val;
    
    // vpextrq rcx, xmm0, 1
    rcx_extract = (uint64_t)_mm_extract_epi64(xmm0, 1);
    
    // xor rdx, rcx
    rdx_hash ^= rcx_extract;
    
    // xor rdx, [rbx+40h]
    rdx_hash ^= *(volatile uint64_t*)((uint8_t*)rbx + 0x40);
    
    // vmovq rcx, xmm1
    rcx_extract = (uint64_t)_mm_cvtsi128_si64(xmm1);
    
    // xor rdx, rcx
    rdx_hash ^= rcx_extract;
    
    // mov rcx, [rbx+88h] - get inner object pointer
    void* inner = *(void* volatile*)((uint8_t*)rbx + 0x88);
    
    // xor rdx, rax
    rdx_hash ^= rax_val;
    
    // mov rax, 1234567898765432h
    uint64_t magic = 0x1234567898765432ULL;
    
    // vmovups [rsp+188h+var_168+18h], xmm0
    _mm_storeu_si128((__m128i*)(var_168 + 0x18), xmm0);
    
    // vmovups [rsp+188h+var_168+8], xmm1
    _mm_storeu_si128((__m128i*)(var_168 + 0x08), xmm1);
    
    // vmovups ymm0, [rsp+188h+var_168]
    ymm0 = _mm256_loadu_ps((float*)var_168);
    
    // vmovups xmm1, [rsp+40h] - load from fixed stack offset
    // This is at rsp+0x40 in the original, which would be stack_frame[0x40]
    __m128 xmm1_extra = _mm_loadu_ps((float*)&stack_frame[0x40]);
    
    // xor rdx, rax
    rdx_hash ^= magic;
    
    // mov rax, [rcx] - dereference inner to get vtable
    void** inner_vtable = *(void***)inner;
    
    // mov [rsp+188h+var_138], rdx
    *var_138 = rdx_hash;
    
    // lea rdx, [rsp+188h+var_128]
    // vmovups [rsp+188h+var_128], ymm0
    _mm256_storeu_ps((float*)var_128, ymm0);
    
    // vmovsd xmm0, [rsp+188h+var_138]
    __m128d xmm0_sd = _mm_load_sd((double*)var_138);
    
    // vmovsd [rsp+188h+var_F8], xmm0
    _mm_store_sd((double*)var_F8, xmm0_sd);
    
    // vmovups [rsp+188h+var_108], xmm1
    _mm_storeu_ps((float*)var_108, xmm1_extra);
    
    // vzeroupper
    _mm256_zeroupper();
    
    // ===== VIRTUAL CALL AT [rax+40h] =====
    // call qword ptr [rax+40h]
    typedef void* (*vtable_func_40)(void*, void*);
    vtable_func_40 func = (vtable_func_40)inner_vtable[8]; // offset 0x40 = index 8
    void* call_result = func(inner, (void*)var_128);
    
    // mov [rsp+188h+var_90], rax
    *var_90 = (uint64_t)call_result;
    
    // test rax, rax
    // jnz short loc_140551131
    if (call_result == NULL)
    {
        // lea r8, [rsp+188h+var_E8]
        // mov rcx, rbx
        // lea rdx, [rsp+188h+var_168]
        // call Subsumption__Task__InitializeTask
        Subsumption__Task__InitializeTask(rbx, (void*)var_168, (void*)var_E8);
    }
    
    // loc_140551131:
    // mov rax, [rbx]
    void** main_vtable = *(void***)rbx;
    
    // lea rdx, [rsp+188h+var_E8]
    // mov r8, rdi
    // mov rcx, rbx
    // call qword ptr [rax+88h]
    typedef int64_t (*vtable_func_88)(void*, void*, void*);
    vtable_func_88 final_func = (vtable_func_88)main_vtable[17]; // offset 0x88 = index 17
    int64_t result = final_func(rbx, (void*)var_E8, rdi);
    
    return result;
}

// External function stub
__attribute__((noinline))
void Subsumption__Task__InitializeTask(void* rcx, void* rdx, void* r8)
{
    FORCE_USE(rcx);
    FORCE_USE(rdx);
    FORCE_USE(r8);
}

// Test vtable methods
__attribute__((noinline))
void* inner_method_40(void* self, void* param)
{
    FORCE_USE(self);
    FORCE_USE(param);
    return (void*)0;
}

__attribute__((noinline))
int64_t main_method_88(void* self, void* data, void* r8_param)
{
    FORCE_USE(self);
    FORCE_USE(data);
    FORCE_USE(r8_param);
    return 0x42;
}

// Test harness
int main(void)
{
    // Setup vtables
    void* inner_vt[9] = {0};
    inner_vt[8] = (void*)inner_method_40;  // offset 0x40
    
    void* main_vt[18] = {0};
    main_vt[17] = (void*)main_method_88;   // offset 0x88
    
    // Inner object
    void* inner_obj[1];
    inner_obj[0] = inner_vt;
    
    // Main context - 0x90 bytes to cover offset 0x88
    uint8_t ctx[0x90] __attribute__((aligned(8))) = {0};
    *(void**)&ctx[0x00] = main_vt;          // vtable
    *(uint64_t*)&ctx[0x40] = 0xDEADBEEFCAFEBABEULL;
    *(void**)&ctx[0x88] = inner_obj;
    // Set xmm1 source at 0x38
    uint64_t xmm1_lo = 0x3333333344444444ULL;
    uint64_t xmm1_hi = 0x1111111122222222ULL;
    memcpy(&ctx[0x38], &xmm1_lo, 8);
    memcpy(&ctx[0x40], &xmm1_hi, 8);  // Note: overlaps field_40
    
    // Source data 0xE0 bytes
    uint8_t src[0xE0] __attribute__((aligned(32)));
    for (int i = 0; i < 0xE0; i++) {
        src[i] = (uint8_t)(i * 7);
    }
    
    // Call
    int64_t result = sub_140550FF0(ctx, src, (void*)0xABCD1234);
    
    return (int)result;
}
