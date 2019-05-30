// adapted to this project by D. Lemire, from https://github.com/wangyi-fudan/wyhash/blob/master/wyhash.h
// This uses mum hashing.
#include <stdint.h>
// state for wyrand
uint64_t wyrand_x; /* The state can be seeded with any value. */

// call wyrand_seed before calling wyrand
static inline void wyrand_seed(uint64_t seed) { wyrand_x = seed; }

static inline uint64_t wyrand_stateless(uint64_t *s) {
  *s += UINT64_C(0xa0761d6478bd642f);
  __uint128_t t = (__uint128_t)*s * (*s ^ UINT64_C(0xe7037ed1a0b428db));
  return (t >> 64) ^ t;
}

// returns random number, modifies wyrand_x
static inline uint64_t wyrand(void) { return wyrand_stateless(&wyrand_x); }

// returns the 32 least significant bits of a call to wyrand
// this is a simple function call followed by a cast
static inline uint32_t wyrand_cast32(void) { return (uint32_t)wyrand(); }
