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
produces separate stream of a period of at least 2^64. 
 
For full independence of 2^64 streams, the first 48 outputs of each stream should be skipped, 
otherwise minor correlations may occur in the first outputs on the low bits of successively 
initialized streams, especially if one initalize streams by numbers 1, 3, 5, ... or other counter. 
For this purpose user may call initalizator(). Using initalizator(), each stream may be initialized 
with any 64-bit odd number. Skipping the first few outputs is also required to fill all the state
of the generator by bits, however this can be also done by proper seed selection. 
 
For faster initialization user may seed a Splitmix63 generator (a modified Splitmix64 generator) 
and use its outputs to serially fill c[0].

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

/* This is Splitmix63, modified Splitmix64 written in 2015 by Sebastiano Vigna. The state of Splitmix63
may be seeded with any value. Outputs of Splitmix63 may be used to to serially seed c[0] states
of Collatz Generator by c[0] = (next_splitmix63() << 1) | 1; */
 
static uint64_t CG64_y = 123;

uint64_t next_splitmix63(void) {
 	uint64_t CG64_z = (CG64_y += 0x9e3779b97f4a7c15) & 0x7fffffffffffffff;
	CG64_z = ((CG64_z ^ (CG64_z >> 30)) * 0xbf58476d1ce4e5b9) & 0x7fffffffffffffff;
	CG64_z = ((CG64_z ^ (CG64_z >> 27)) * 0x94d049bb133111eb) & 0x7fffffffffffffff;
	return CG64_z ^ (CG64_z >> 31);
}

CG64_c[0] = (next_splitmix63() << 1) | 1;

static inline uint64_t CG64(void)
{
    CG64_c[1] = (CG64_c[1] >> 1) * ((CG64_c[2] += CG64_c[1]) | 1) ^ (CG64_c[3] += CG64_c[0]);
    return CG64_c[2] >> 48 ^ CG64_c[1];
}

#endif // CG64_H
