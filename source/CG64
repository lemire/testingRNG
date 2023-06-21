#ifndef CG64_H
#define CG64_H

#include "splitmix64.h"
#include <stdint.h>

static uint64_t CG64_c[4];

// call this one before calling CG64
static inline void CG64_seed(uint64_t seed) {
  CG64_c[0] = splitmix64_r(&seed) | 1;
}

static inline uint64_t CG64(void)
{
    CG64_c[1] = (CG64_c[1] >> 1) * ((CG64_c[2] += CG64_c[1]) | 1) ^ (CG64_c[3] += CG64_c[0]);
    return CG64_c[2] >> 48 ^ CG64_c[1];
}

#endif // CG64_H
