#ifndef CG128_H
#define CG128_H

#include "splitmix64.h"
#include <stdint.h>

static __uint128_t CG128_c[4];

// call this one before calling CG128
static inline void CG128_seed(uint64_t seed) {
  CG128_c[0] = ((__uint128_t)splitmix64_r(&seed) << 64) | (splitmix64_r(&seed) | 1);
}

static inline __uint128_t CG128(void)
{
    CG128_c[1] = (CG128_c[1] >> 1) * ((CG128_c[2] += CG128_c[1]) | 1) ^ (CG128_c[3] += CG128_c[0]);
    return CG128_c[2] >> 96 ^ CG128_c[1];
}

#endif // CG128_H
