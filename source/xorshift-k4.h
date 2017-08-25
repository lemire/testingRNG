#ifndef XORSHIFTK4_H
#define XORSHIFTK4_H

// adapted from code by Piotr Stefaniak

/* Marsaglia, George (July 2003). "Xorshift RNGs".
 * Journal of Statistical Software. Vol. 8 (Issue  14). */

#include "splitmix64.h"

static uint32_t xorshift_k4_x, xorshift_k4_y, xorshift_k4_z, xorshift_k4_w;

static inline void xorshift_k4_seed(uint64_t seed) {
  xorshift_k4_x = splitmix64_stateless(seed);
  xorshift_k4_y = splitmix64_stateless(seed + 1);
  xorshift_k4_z = splitmix64_stateless(seed + 2);
  xorshift_k4_w = splitmix64_stateless(seed + 3);
  return;
}

static inline uint32_t xorshift_k4(void) {
  uint32_t t = xorshift_k4_x ^ (xorshift_k4_x << 11);
  xorshift_k4_x = xorshift_k4_y;
  xorshift_k4_y = xorshift_k4_z;
  xorshift_k4_z = xorshift_k4_w;
  return xorshift_k4_w = xorshift_k4_w ^ (xorshift_k4_w >> 19) ^ (t ^ (t >> 8));
}

#endif
