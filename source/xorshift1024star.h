#ifndef XORSHIFT1024STAR_H
#define XORSHIFT1024STAR_H

/* Modified by D. Lemire, August 2017 */
#include "splitmix64.h"
#include <stdint.h>

/*  Written in 2014 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

/* This is a fast, top-quality generator. If 1024 bits of state are too
   much, try a xoroshiro128+ generator.

   Note that the three lowest bits of this generator are LSFRs, and thus
   they are slightly less random than the other bits. We suggest to use a
   sign test to extract a random Boolean value.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

#define xorshift1024star_size 16
static uint64_t xorshift1024star_s[xorshift1024star_size];
static int xorshift1024star_p;

// call once before calling xorshift1024star
static inline void xorshift1024star_seed(uint64_t seed) {
  xorshift1024star_p = 0;
  for (int k = 0; k < xorshift1024star_size; k++)
    xorshift1024star_s[k] = splitmix64_stateless(seed + k);
}

// returns random number, modifies xorshift1024star_s and xorshift1024star_p
static inline uint64_t xorshift1024star(void) {
  const uint64_t s0 = xorshift1024star_s[xorshift1024star_p];
  xorshift1024star_p = (xorshift1024star_p + 1) % xorshift1024star_size;
  uint64_t s1 = xorshift1024star_s[xorshift1024star_p];
  s1 ^= s1 << 31; // a
  xorshift1024star_s[xorshift1024star_p] =
      s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b,c
  return xorshift1024star_s[xorshift1024star_p] * UINT64_C(1181783497276652981);
}

#endif // XORSHIFT1024STAR_H
