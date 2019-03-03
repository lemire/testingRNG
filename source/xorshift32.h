#ifndef XORSHIFT32_H
#define XORSHIFT32_H

#include "splitmix64.h"
#include <stdint.h>

static unsigned int xorshift_y;

void xorshift32_seed(uint64_t seed) { xorshift_y = seed; }

unsigned int xorshift32(void) {
  xorshift_y ^= (xorshift_y << 13);
  xorshift_y ^= (xorshift_y >> 17);
  return xorshift_y ^= (xorshift_y << 5);
}

#endif // XORSHIFT32_H
