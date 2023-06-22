#ifndef CG128_H
#define CG128_H

#include "splitmix64.h"
#include <stdint.h>

/* Written in 2023 by Tomasz R. Dziala (tomasz-dziala@wp.pl).
 
This is 128-bit Collatz Generator: CG-128. All purpose pseudo random generator,
with excellent statistical quality (it passes all test we aware of) and superb speed.
 
The state may be seeded with any numbers, except c[0], which has to be odd, but we suggest
to seed c[0] by 128-bit odd number and set all other elements equal to zero, for simplicity. 
It is suitable for large-scale parallel computations, since each generator seeded by unike 
odd c[0] produce separate stream of period at least 2^128. 
 
For full independence of 2^64 streams, the first 32 outputs of each stream should be skipped, 
otherwise minor correlations may occur in the first outputs on the low bits of successively 
initialized streams, especially if one initalize streams by numbers 1, 3, 5, ... or other counter. 
For this purpose user may call initalizator(). Using initalizator() every stream may be initialized 
with any 128-bit odd number. If you ever need more than 2^64 independent streams skip first 96 outputs
to get 2^127 independent streams. Skipping the first few outputs is also required to fill all the state
of the generator by bits, however this can be also done by proper choose of the seed. 
 
For faster initialization user may seed a Splitmix64 generator and use its outputs to serially fill c[0]. 

This is initalizator for the Collatz Generator. It is equivalent to 32 calls to next();
it could be used for initializing independent streams for parallel computations.
 
void initializator(void)
{
    for (int i = 0; i < 32; i++)
    {
        c[1] = (c[1] >> 1) * ((c[2] += c[1]) | 1) ^ (c[3] += c[0]);
    }
}*/

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
