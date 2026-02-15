#include "common.h"

NOINLINE double test_vaddsd_alias_op3(double a, double b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vaddsd %0, %1, %0"
                 : "+x"(b)
                 : "x"(a));
#else
    b = a + b;
#endif
    return b;
}

NOINLINE double test_vsubsd_alias_op3(double a, double b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vsubsd %0, %1, %0"
                 : "+x"(b)
                 : "x"(a));
#else
    b = a - b;
#endif
    return b;
}

NOINLINE double test_vmulsd_alias_op3(double a, double b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vmulsd %0, %1, %0"
                 : "+x"(b)
                 : "x"(a));
#else
    b = a * b;
#endif
    return b;
}

NOINLINE double test_vdivsd_alias_op3(double a, double b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vdivsd %0, %1, %0"
                 : "+x"(b)
                 : "x"(a));
#else
    b = a / b;
#endif
    return b;
}

NOINLINE double test_vaddsd_mem_op3(double a, double b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vaddsd %1, %0, %0"
                 : "+x"(a)
                 : "m"(b));
#else
    a = a + b;
#endif
    return a;
}

NOINLINE double test_vsubsd_mem_op3(double a, double b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vsubsd %1, %0, %0"
                 : "+x"(a)
                 : "m"(b));
#else
    a = a - b;
#endif
    return a;
}

NOINLINE double test_vmulsd_mem_op3(double a, double b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vmulsd %1, %0, %0"
                 : "+x"(a)
                 : "m"(b));
#else
    a = a * b;
#endif
    return a;
}

NOINLINE double test_vdivsd_mem_op3(double a, double b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vdivsd %1, %0, %0"
                 : "+x"(a)
                 : "m"(b));
#else
    a = a / b;
#endif
    return a;
}

NOINLINE float test_vaddss_alias_op3(float a, float b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vaddss %0, %1, %0"
                 : "+x"(b)
                 : "x"(a));
#else
    b = a + b;
#endif
    return b;
}

NOINLINE float test_vsubss_alias_op3(float a, float b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vsubss %0, %1, %0"
                 : "+x"(b)
                 : "x"(a));
#else
    b = a - b;
#endif
    return b;
}

NOINLINE float test_vmulss_alias_op3(float a, float b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vmulss %0, %1, %0"
                 : "+x"(b)
                 : "x"(a));
#else
    b = a * b;
#endif
    return b;
}

NOINLINE float test_vdivss_alias_op3(float a, float b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vdivss %0, %1, %0"
                 : "+x"(b)
                 : "x"(a));
#else
    b = a / b;
#endif
    return b;
}

NOINLINE float test_vaddss_mem_op3(float a, float b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vaddss %1, %0, %0"
                 : "+x"(a)
                 : "m"(b));
#else
    a = a + b;
#endif
    return a;
}

NOINLINE float test_vsubss_mem_op3(float a, float b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vsubss %1, %0, %0"
                 : "+x"(a)
                 : "m"(b));
#else
    a = a - b;
#endif
    return a;
}

NOINLINE float test_vmulss_mem_op3(float a, float b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vmulss %1, %0, %0"
                 : "+x"(a)
                 : "m"(b));
#else
    a = a * b;
#endif
    return a;
}

NOINLINE float test_vdivss_mem_op3(float a, float b) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("vdivss %1, %0, %0"
                 : "+x"(a)
                 : "m"(b));
#else
    a = a / b;
#endif
    return a;
}

int main(void) {
    volatile double d_add = test_vaddsd_alias_op3(1.25, 2.5);
    volatile double d_sub = test_vsubsd_alias_op3(9.0, 2.5);
    volatile double d_mul = test_vmulsd_alias_op3(3.0, 2.0);
    volatile double d_div = test_vdivsd_alias_op3(9.0, 3.0);

    volatile float f_add = test_vaddss_alias_op3(1.5f, 2.25f);
    volatile float f_sub = test_vsubss_alias_op3(9.0f, 2.25f);
    volatile float f_mul = test_vmulss_alias_op3(3.0f, 2.0f);
    volatile float f_div = test_vdivss_alias_op3(7.5f, 2.5f);

    volatile double md_add = test_vaddsd_mem_op3(1.25, 2.5);
    volatile double md_sub = test_vsubsd_mem_op3(9.0, 2.5);
    volatile double md_mul = test_vmulsd_mem_op3(3.0, 2.0);
    volatile double md_div = test_vdivsd_mem_op3(9.0, 3.0);

    volatile float mf_add = test_vaddss_mem_op3(1.5f, 2.25f);
    volatile float mf_sub = test_vsubss_mem_op3(9.0f, 2.25f);
    volatile float mf_mul = test_vmulss_mem_op3(3.0f, 2.0f);
    volatile float mf_div = test_vdivss_mem_op3(7.5f, 2.5f);

    printf("sd: add=%f sub=%f mul=%f div=%f\n", d_add, d_sub, d_mul, d_div);
    printf("ss: add=%f sub=%f mul=%f div=%f\n",
           (double)f_add,
           (double)f_sub,
           (double)f_mul,
           (double)f_div);
    printf("sd(mem): add=%f sub=%f mul=%f div=%f\n", md_add, md_sub, md_mul, md_div);
    printf("ss(mem): add=%f sub=%f mul=%f div=%f\n",
           (double)mf_add,
           (double)mf_sub,
           (double)mf_mul,
           (double)mf_div);
    return 0;
}
