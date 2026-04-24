#pragma once

#include "../common.h"

#define DEFINE_PS_UNARY(name, intrinsic) \
    NOINLINE __m256 name(__m256 a) { return intrinsic(a); }

#define DEFINE_PS_BINARY(name, intrinsic) \
    NOINLINE __m256 name(__m256 a, __m256 b) { return intrinsic(a, b); }

#define DEFINE_PS_TERNARY(name, intrinsic) \
    NOINLINE __m256 name(__m256 a, __m256 b, __m256 c) { return intrinsic(a, b, c); }

#define DEFINE_INT_UNARY(name, intrinsic) \
    NOINLINE __m256i name(__m256i a) { return intrinsic(a); }

#define DEFINE_INT_BINARY(name, intrinsic) \
    NOINLINE __m256i name(__m256i a, __m256i b) { return intrinsic(a, b); }
