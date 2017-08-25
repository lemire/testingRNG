#ifndef XORSHIFT128PLUS_H
#define XORSHIFT128PLUS_H

#include <stdint.h>

#include "splitmix64.h"

/* Keys for scalar xorshift128. Must be non-zero
These are modified by xorshift128plus.
 */
struct xorshift128plus_key_s {
  uint64_t part1;
  uint64_t part2;
};

typedef struct xorshift128plus_key_s xorshift128plus_key_t;

static xorshift128plus_key_t global_xorshift128plus_key;

static inline void xorshift128plus_init(uint64_t key1, uint64_t key2,
                                        xorshift128plus_key_t *key) {
  key->part1 = key1;
  key->part2 = key2;
}

static inline void xorshift128plus_seed(uint64_t seed) {
  xorshift128plus_init(splitmix64_stateless(seed),
                       splitmix64_stateless(seed + 1),
                       &global_xorshift128plus_key);
}

uint64_t xorshift128plus_r(xorshift128plus_key_t *key) {
  uint64_t s1 = key->part1;
  const uint64_t s0 = key->part2;
  key->part1 = s0;
  s1 ^= s1 << 23;                                // a
  key->part2 = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
  return key->part2 + s0;
}

static inline uint64_t xorshift128plus() {
  return xorshift128plus_r(&global_xorshift128plus_key);
}

#endif
