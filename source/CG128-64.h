#ifndef CG128_64_H
#define CG128_64_H

#include "splitmix64.h"
#include "splitmix63.h"
#include <stdint.h>

/* Written in 2023 by Tomasz R. Dziala (tomasz-dziala@wp.pl).
 
This is a 128-64-bit Collatz Generator: CG-128-64. All purpose pseudorandom generator,
with excellent statistical quality (it passes all tests I'm aware of) and superb speed.
 
The state may be seeded with any numbers, except s, which must be odd, but I suggest
to seed s by a 64-bit odd number and set all other elements equal zero, for simplicity.
It is suitable for large-scale parallel computations, since each generator seeded by unike 
odd s produces separate stream of a period of at least 2^64. 
 
For full independence of 2^64 streams, the first 48 outputs of each stream should be skipped,
otherwise minor correlations may occur in the first outputs on the low bits of successively 
initialized streams, especially if one initalize the streams by numbers 1, 3, 5, ... or other counter. 
For this purpose user may call initalizator(). Using initalizator(), each stream can be initialized 
with any 64-bit odd number. Skipping the first few outputs is also required to fill all the state 
of the generator by bits, however this can be also done by proper seed selection.
 
For faster initialization user may use a Splitmix64 generator and a Splitmix63 generator 
(a modified Splitmix64 generator) and use its outputs to serially fill x and s. */

/* This is initalizator for the Collatz Generator. It is equivalent to 48 calls to next();
it could be used for initializing independent streams for parallel computations.
 
void initializator(void)
{
    for (int i = 0; i < 48; i++)
    {
        x = (x | 1) * ((k += x) >> 1) ^ (weyl += s);
    }
} */

static uint64_t CG_s, CG_k = 0, CG_weyl = 0;
static __uint128_t CG_x;

// call this to seed CG_x and CG_s
static inline void CG128_64_seed(uint64_t seed) {
	CG_x = ((__uint128_t)splitmix64_r(&seed) << 64) | (splitmix64_r(&seed) | 1);
	CG_s = (splitmix63_r(&seed) << 1) | 1;
}

static inline __uint128_t CG128_64(void)
{
    CG_x = (CG_x | 1) * ((CG_k += CG_x) >> 1) ^ (CG_weyl += CG_s);
    return CG_k >> 48 ^ CG_x;
}

#endif // CG128_64_H
