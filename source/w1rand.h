// w1rand: a wyrand-style PRNG by wangyi-fudan.
// Uses a single constant for both the increment and the XOR mix.
// See: https://github.com/alvoskov/SmokeRand/blob/devel/generators/w1rand.c
//      https://github.com/lemire/testingRNG/issues/28
#include <stdint.h>

static const uint64_t w1rand_c = UINT64_C(0xd07ebc63274654c7);

uint64_t w1rand_x;

static inline void w1rand_seed(uint64_t seed) { w1rand_x = seed; }

static inline uint64_t w1rand_stateless(uint64_t *s) {
  *s += w1rand_c;
  __uint128_t t = (__uint128_t)*s * (*s ^ w1rand_c);
  return (uint64_t)(t >> 64) ^ (uint64_t)t;
}

static inline uint64_t w1rand(void) { return w1rand_stateless(&w1rand_x); }

static inline uint32_t w1rand_cast32(void) { return (uint32_t)w1rand(); }
