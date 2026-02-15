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

int main(void) {
    volatile double d_add = test_vaddsd_alias_op3(1.25, 2.5);
    volatile double d_sub = test_vsubsd_alias_op3(9.0, 2.5);
    volatile double d_mul = test_vmulsd_alias_op3(3.0, 2.0);
    volatile double d_div = test_vdivsd_alias_op3(9.0, 3.0);

    volatile float f_add = test_vaddss_alias_op3(1.5f, 2.25f);
    volatile float f_sub = test_vsubss_alias_op3(9.0f, 2.25f);
    volatile float f_mul = test_vmulss_alias_op3(3.0f, 2.0f);
    volatile float f_div = test_vdivss_alias_op3(7.5f, 2.5f);

    printf("sd: add=%f sub=%f mul=%f div=%f\n", d_add, d_sub, d_mul, d_div);
    printf("ss: add=%f sub=%f mul=%f div=%f\n",
           (double)f_add,
           (double)f_sub,
           (double)f_mul,
           (double)f_div);
    return 0;
}
