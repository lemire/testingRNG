#ifndef XORSHIFT128PLUS_H
#define XORSHIFT128PLUS_H

#include <stdint.h>

#include "splitmix64.h"

/* Keys for scalar xorshift128. Must be non-zero
These are modified by xorshift128plus.
 */
struct xorshift128plus_key_s {
  uint64_t part1;
  uint64_t part2;
};

typedef struct xorshift128plus_key_s xorshift128plus_key_t;

static xorshift128plus_key_t global_xorshift128plus_key;

static inline void xorshift128plus_init(uint64_t key1, uint64_t key2,
                                        xorshift128plus_key_t *key) {
  key->part1 = key1;
  key->part2 = key2;
}

static inline void xorshift128plus_seed(uint64_t seed) {
  xorshift128plus_init(splitmix64_stateless(seed),
                       splitmix64_stateless(seed + 1),
                       &global_xorshift128plus_key);
}

// this is the code as found on Vigna's site
// http://xoroshiro.di.unimi.it/xorshift128plus.c (last checked October 4th
// 2017)
// It is also the code that appears in
// Further scramblings of Marsaglia’s xorshift generators
// http://vigna.di.unimi.it/ftp/papers/xorshiftplus.pdf
// Figure 1: The xorshift128+ generator used in the tests.
// Original documentation by Vigna :
/* This generator has been replaced by xoroshiro128plus, which is
   significantly faster and has better statistical properties.

   It might be nonetheless useful for languages in which low-level rotate
   instructions are not available. Due to the relatively short period it
   is acceptable only for applications with a mild amount of parallelism;
   otherwise, use a xorshift1024* generator.

   Note that the lowest bit of this generator is an LSFR, and thus it will
   fail binary rank tests. The next bit is not an LFSR, but in the long
   run it will fail binary rank tests, too. The other bits have no LFSR
   artifacts.

   We suggest to use a sign test to extract a random Boolean value.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s.

   A previous version of this generator was adding the two halves of the
   newly computed state. This version adds the two halves of the *current*
   state (as xoroshiro128plus does), which improves speed due to better
   internal parallelization from the CPU. The resulting streams are off by
   one step. */

uint64_t xorshift128plus_r(xorshift128plus_key_t *key) {
  uint64_t s1 = key->part1;
  const uint64_t s0 = key->part2;
  key->part1 = s0;
  s1 ^= s1 << 23;                                // a
  key->part2 = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
  return key->part2 + s0;
}

// this is the version found on Wikipedia
// https://en.wikipedia.org/wiki/Xorshift#xorshift.2B (Oct 4th 2018)
// as well as the version reported to be used by the v8 project
// https://v8project.blogspot.ca/2015/12/theres-mathrandom-and-then-theres.html
uint64_t v8xorshift128plus_r(xorshift128plus_key_t *key) {
  uint64_t s1 = key->part1;
  const uint64_t s0 = key->part2;
  key->part1 = s0;
  s1 ^= s1 << 23;                                 // a
  key->part2 = s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26); // b, c
  return key->part2 + s0;
}

static inline uint64_t xorshift128plus() {
  return xorshift128plus_r(&global_xorshift128plus_key);
}

static inline uint64_t v8xorshift128plus() {
  return v8xorshift128plus_r(&global_xorshift128plus_key);
}

#endif
