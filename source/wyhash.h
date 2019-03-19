// adapted to this project by D. Lemire, from https://github.com/wangyi-fudan/wyhash/blob/master/wyhash.h
// This uses mum hashing.
#include <stdint.h>
// state for wyhash64
uint64_t wyhash64_x; /* The state can be seeded with any value. */

// call wyhash64_seed before calling wyhash64
static inline void wyhash64_seed(uint64_t seed) { wyhash64_x = seed; }

static inline uint64_t wyhash64_stateless(uint64_t *seed) {
  *seed += UINT64_C(0x60bee2bee120fc15);
  __uint128_t tmp;
  tmp = (__uint128_t)*seed * UINT64_C(0xa3b195354a39b70d);
  uint64_t m1 = (tmp >> 64) ^ tmp;
  tmp = (__uint128_t)m1 * UINT64_C(0x1b03738712fad5c9);
  uint64_t m2 = (tmp >> 64) ^ tmp;
  return m2;
}

// returns random number, modifies wyhash64_x
static inline uint64_t wyhash64(void) { return wyhash64_stateless(&wyhash64_x); }

// returns the 32 least significant bits of a call to wyhash64
// this is a simple function call followed by a cast
static inline uint32_t wyhash64_cast32(void) { return (uint32_t)wyhash64(); }
