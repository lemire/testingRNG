#ifndef SPLITMIX63_H
#define SPLITMIX63_H

/* This is Splitmix63, modified Splitmix64 written in 2015 by Sebastiano Vigna. The state of Splitmix63
may be seeded with any value. */

#include <stdint.h>

// state for splitmix63
uint64_t splitmix63_x; /* The state can be seeded with any value. */

// call this one before calling splitmix64
static inline void splitmix63_seed(uint64_t seed) { splitmix63_x = seed; }

// floor( ( (1+sqrt(5))/2 ) * 2**64 MOD 2**64)
#define GOLDEN_GAMMA UINT64_C(0x9E3779B97F4A7C15)

static inline uint64_t splitmix63_r(uint64_t *seed) {
  uint64_t z = (*seed += GOLDEN_GAMMA) & 0x7fffffffffffffff;
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9) & 0x7fffffffffffffff;
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB) & 0x7fffffffffffffff;
  return z ^ (z >> 31);
}

static inline uint64_t splitmix63(void) {
    return splitmix63_r(&splitmix63_x);
}

// returns the 32 least significant bits of a call to splitmix64
// this is a simple (inlined) function call followed by a cast
static inline uint32_t splitmix63_cast32(void) {
  return (uint32_t)splitmix63();
}

// returns the value of splitmix64 "offset" steps from seed
static inline uint64_t splitmix63_stateless(uint64_t seed, uint64_t offset) {
  seed += offset*GOLDEN_GAMMA;
  return splitmix63_r(&seed);
}

#endif // SPLITMIX63_H
