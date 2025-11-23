#include "avx_test.h"
#include <cpuid.h>

// Safety check for manual compilation attempts on Apple Silicon
#if defined(__APPLE__) && defined(__aarch64__)
#error "AVX instructions are not supported on ARM64 native. Please compile with -arch x86_64 to use Rosetta 2."
#endif

void check_cpu_support() {
    unsigned int eax, ebx, ecx, edx;

    // Check for AVX
    if (!__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
        fprintf(stderr, "Error: CPUID failed.\n");
        exit(1);
    }

    if (!(ecx & bit_AVX)) {
        fprintf(stderr, "Error: AVX not supported by this CPU (or emulation layer).\n");
        exit(1);
    }

    // Check for AVX2
    if (__get_cpuid_max(0, NULL) < 7) {
        fprintf(stderr, "Error: CPUID level 7 not supported.\n");
        exit(1);
    }

    __cpuid_count(7, 0, eax, ebx, ecx, edx);
    if (!(ebx & bit_AVX2)) {
        fprintf(stderr, "Error: AVX2 not supported by this CPU (or emulation layer).\n");
        exit(1);
    }

    printf("CPU Support Verified: AVX, AVX2\n");
}

void init_test_data(TestData *data) {
    // Initialize with non-zero values to avoid div-by-zero where applicable
    // and varied bit patterns for logic ops.
    for (int i = 0; i < 16; i++) data->f32[i] = (float)(i + 1.5f);
    for (int i = 0; i < 8; i++) data->f64[i] = (double)(i + 1.5);
    for (int i = 0; i < 16; i++) data->i32[i] = (i + 1) * 10;
    for (int i = 0; i < 8; i++) data->i64[i] = (long long)(i + 1) * 100;
    for (int i = 0; i < 32; i++) data->i16[i] = (short)(i + 1) * 5;
    for (int i = 0; i < 64; i++) data->i8[i] = (char)(i + 1);
}

int main() {
    check_cpu_support();

    TestData *data = _mm_malloc(sizeof(TestData), 32);
    if (!data) {
        perror("Memory allocation failed");
        return 1;
    }

    init_test_data(data);

    printf("Executing AVX Operations...\n");
    run_avx_ops(data);

    printf("Executing AVX2 Operations...\n");
    run_avx2_ops(data);

    printf("All operations executed successfully.\n");

    _mm_free(data);
    return 0;
}
