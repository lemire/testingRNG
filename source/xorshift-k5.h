#ifndef XORSHIFTK5_H
#define XORSHIFTK5_H

// adapted from code by Piotr Stefaniak

/* George Marsaglia,
   https://groups.google.com/d/msg/comp.lang.c/qZFQgKRCQGg/rmPkaRHqxOMJ */

#include "splitmix64.h"

static uint32_t xorshift_k5_x, xorshift_k5_y, xorshift_k5_z, xorshift_k5_w,
    xorshift_k5_v;

static inline void xorshift_k5_seed(uint64_t seed) {
  xorshift_k5_x = splitmix64_stateless(seed);
  xorshift_k5_y = splitmix64_stateless(seed + 1);
  xorshift_k5_z = splitmix64_stateless(seed + 2);
  xorshift_k5_w = splitmix64_stateless(seed + 3);
  xorshift_k5_v = splitmix64_stateless(seed + 4);
  return;
}

static inline uint32_t xorshift_k5(void) {
  uint32_t t = (xorshift_k5_x ^ (xorshift_k5_x >> 7));
  xorshift_k5_x = xorshift_k5_y;
  xorshift_k5_y = xorshift_k5_z;
  xorshift_k5_z = xorshift_k5_w;
  xorshift_k5_w = xorshift_k5_v;
  xorshift_k5_v = (xorshift_k5_v ^ (xorshift_k5_v << 6)) ^ (t ^ (t << 13));
  return (xorshift_k5_y + xorshift_k5_y + 1) * xorshift_k5_v;
}

#endif
