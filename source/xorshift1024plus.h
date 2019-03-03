#ifndef XORSHIFT1024PLUS_H
#define XORSHIFT1024PLUS_H

#include "splitmix64.h"
#include <stdint.h>

/* Figure 2 in Further scramblings of Marsaglia’s xorshift generators
 by Vigna.
 */

#define xorshift1024plus_size 16
static uint64_t xorshift1024plus_s[xorshift1024plus_size];
static int xorshift1024plus_p;

// call once before calling xorshift1024plus
static inline void xorshift1024plus_seed(uint64_t seed) {
  xorshift1024plus_p = 0;
  for (int k = 0; k < xorshift1024plus_size; k++)
    xorshift1024plus_s[k] = splitmix64_stateless(seed + k);
}
// returns random number, modifies xorshift1024plus_s and xorshift1024plus_p
static inline uint64_t xorshift1024plus(void) {
  const uint64_t s0 = xorshift1024plus_s[xorshift1024plus_p];
  uint64_t s1 =
      xorshift1024plus_s[xorshift1024plus_p = (xorshift1024plus_p + 1) & 15];
  const uint64_t result = s0 + s1;
  s1 ^= s1 << 31; // a
  xorshift1024plus_s[xorshift1024plus_p] =
      s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b, c
  return result;
}

#endif // XORSHIFT1024PLUS_H
