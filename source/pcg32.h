#ifndef PCG32_H
#define PCG32_H

/* Modified by D. Lemire based on original code by M. O'Neill, August 2017 */
#include "splitmix64.h"
#include <stdint.h>

struct pcg_state_setseq_64 {
  uint64_t
      state;    // RNG state.  All values are possible.  Will change over time.
  uint64_t inc; // Controls which RNG sequence (stream) is
  // selected. Must *always* be odd. Might be changed by the user, never by our
  // code.
};

typedef struct pcg_state_setseq_64 pcg32_random_t;

static pcg32_random_t pcg32_global; // global state

// call this once before calling pcg32_random_r
static inline void pcg32_seed(uint64_t seed) {
  pcg32_global.state = splitmix64_stateless(seed);
  // we pick a sequence at random
  pcg32_global.inc =
      (splitmix64_stateless(seed + 1)) | 1; // making sure it is odd
}

static inline uint32_t pcg32_random_r(pcg32_random_t *rng) {
  uint64_t oldstate = rng->state;
  rng->state = oldstate * UINT64_C(0x5851f42d4c957f2d) + rng->inc;
  uint32_t xorshifted = ((oldstate >> 18) ^ oldstate) >> 27;
  uint32_t rot = oldstate >> 59;
  return (xorshifted >> rot) | (xorshifted << (32 - rot));
}

static inline uint32_t pcg32(void) { return pcg32_random_r(&pcg32_global); }

#endif // PCG32_H
