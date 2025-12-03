/*
 * Random Number Generator
 * Fast xorshift32 PRNG with utility functions.
 */

#ifndef SHOOTER_RNG_H
#define SHOOTER_RNG_H

#include <stdint.h>

/* Core xorshift32 generator */
static inline uint32_t xorshift32(uint32_t* state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

/* Generate random float in [0, 1) */
static inline float randf(uint32_t* state) {
    return (float)(xorshift32(state) & 0xFFFFFF) / (float)0xFFFFFF;
}

/* Generate random float in [min, max) */
static inline float randf_range(uint32_t* state, float min, float max) {
    return min + randf(state) * (max - min);
}

/* Generate random integer in [min, max] (inclusive) */
static inline int randi_range(uint32_t* state, int min, int max) {
    return min + (int)(xorshift32(state) % (uint32_t)(max - min + 1));
}

#endif /* SHOOTER_RNG_H */
