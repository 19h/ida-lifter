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

int main(void) {
    volatile double d = test_vaddsd_alias_op3(1.25, 2.5);
    volatile float f = test_vaddss_alias_op3(1.5f, 2.25f);
    printf("d=%f f=%f\n", (double)d, (double)f);
    return 0;
}
