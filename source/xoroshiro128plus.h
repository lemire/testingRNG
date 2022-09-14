#ifndef XOROSHIRO128PLUS_H
#define XOROSHIRO128PLUS_H

/* Modified by D. Lemire, August 2017 */
#include "splitmix64.h"
#include <stdint.h>

// original documentation by Vigna:
/* This is the successor to xorshift128+. It is the fastest full-period
   generator passing BigCrush without systematic failures, but due to the
   relatively short period it is acceptable only for applications with a
   mild amount of parallelism; otherwise, use a xorshift1024* generator.

   Beside passing BigCrush, this generator passes the PractRand test suite
   up to (and included) 16TB, with the exception of binary rank tests,
   which fail due to the lowest bit being an LFSR; all other bits pass all
   tests. We suggest to use a sign test to extract a random Boolean value.

   Note that the generator uses a simulated rotate operation, which most C
   compilers will turn into a single instruction. In Java, you can use
   Long.rotateLeft(). In languages that do not make low-level rotation
   instructions accessible xorshift128+ could be faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

// state for xoroshiro128plus
uint64_t xoroshiro128plus_s[2];

static inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

// call this one before calling xoroshiro128plus
static inline void xoroshiro128plus_seed(uint64_t seed) {
  xoroshiro128plus_s[0] = splitmix64_r(&seed);
  xoroshiro128plus_s[1] = splitmix64_r(&seed);
}

// returns random number, modifies xoroshiro128plus_s
static inline uint64_t xoroshiro128plus(void) {
  const uint64_t s0 = xoroshiro128plus_s[0];
  uint64_t s1 = xoroshiro128plus_s[1];
  const uint64_t result = s0 + s1;

  s1 ^= s0;
  xoroshiro128plus_s[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14); // a, b
  xoroshiro128plus_s[1] = rotl(s1, 36);                   // c

  return result;
}

#endif // XOROSHIRO128PLUS_H
