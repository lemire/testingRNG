#ifndef CG64_H
#define CG64_H

#include "splitmix64.h"
#include <stdint.h>

/* Written in 2023 by Tomasz R. Dziala (tomasz-dziala@wp.pl).
 
This is 64-bit Collatz Generator: CG-64. All purpose pseudorandom generator,
with excellent statistical quality (it passes all tests I'm aware of) and superb speed.
 
The state may be seeded with any numbers, except c[0], which must be odd, but I suggest
to seed c[0] by 64-bit odd number and set all other elements equal zero, for simplicity.
It is suitable for large-scale parallel computations, since each generator seeded by unike odd c[0]
produce separate stream of period at least 2^64. 
 
For full independence of 2^64 streams, the first 48 outputs of each stream should be skipped, 
otherwise minor correlations may occur in the first outputs on the low bits of successively 
initialized streams, especially if one initalize streams by numbers 1, 3, 5, ... or other counter. 
For this purpose user may call initalizator(). Using initalizator() every stream may be initialized 
with any 64-bit odd number. Skipping the first few outputs is also required to fill all the state
of the generator by bits, however this can be also done by proper choose of the seed. 
 
For faster initialization user may seed a Splitmix64 generator and use its outputs to serially fill c[0]. 

This is initalizator for the Collatz Generator. It is equivalent to 48 calls to next();
it could be used for initializing independent streams for parallel computations.
 
void initializator(void)
{
    for (int i = 0; i < 48; i++)
    {
        c[1] = (c[1] >> 1) * ((c[2] += c[1]) | 1) ^ (c[3] += c[0]);
    }
} */

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
