#ifndef SPLITMIX64_H
#define SPLITMIX64_H

/* This is Splitmix63, modified Splitmix64 written in 2015 by Sebastiano Vigna. The state of Splitmix63
may be seeded with any value. */

#include <stdint.h>
#define GOLDEN_GAMMA UINT64_C(0x9E3779B97F4A7C15)

static inline uint64_t splitmix64_r(uint64_t *seed) {
  uint64_t z = (*seed += GOLDEN_GAMMA) & 0x7fffffffffffffff;
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9) & 0x7fffffffffffffff;
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB) & 0x7fffffffffffffff;
  return z ^ (z >> 31);
}

static inline uint64_t splitmix64(void) {
    return splitmix64_r(&splitmix64_x);
}

static inline uint32_t splitmix64_cast32(void) {
  return (uint32_t)splitmix64();
}

static inline uint64_t splitmix64_stateless(uint64_t seed, uint64_t offset) {
  seed += offset*GOLDEN_GAMMA;
  return splitmix64_r(&seed);
}

#endif // SPLITMIX64_H
