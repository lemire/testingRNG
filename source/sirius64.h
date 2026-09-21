#ifndef SIRIUS64_H
#define SIRIUS64_H

// Sirius64: a 64-bit state, 64-bit output PRNG by Matteo Zapparoli.
// Adapted to this project from https://github.com/matteo65/Sirius64
// (Apache-2.0). See also https://github.com/lemire/testingRNG/issues/30
//
// The state is a Weyl sequence with the golden-ratio increment (as in
// splitmix64); the output function is a multiply-xorshift-rotate mix of the
// state that is not a bijection (it passes 64-bit collision tests).
#include <stdint.h>

#define SIRIUS64_GAMMA UINT64_C(0x9e3779b97f4a7c15)

uint64_t sirius64_x; /* The state can be seeded with any value. */

// call sirius64_seed before calling sirius64
static inline void sirius64_seed(uint64_t seed) { sirius64_x = seed; }

static inline uint64_t sirius64_stateless(uint64_t *s) {
  uint64_t z = (*s += SIRIUS64_GAMMA);
  z = SIRIUS64_GAMMA * (z ^ (z >> 17));
  z = (z << 32) | (z >> 32);
  return SIRIUS64_GAMMA * ((*s) ^ z ^ (z >> 17));
}

// returns random number, modifies sirius64_x
static inline uint64_t sirius64(void) { return sirius64_stateless(&sirius64_x); }

// returns the 32 least significant bits of a call to sirius64
static inline uint32_t sirius64_cast32(void) { return (uint32_t)sirius64(); }

#endif // SIRIUS64_H
