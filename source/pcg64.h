#ifndef PCG64_H
#define PCG64_H

/* Modified by D. Lemire based on original code by M. O'Neill, August 2017 */
#include <stdint.h>
#include "splitmix64.h"

typedef __uint128_t pcg128_t;
#define PCG_128BIT_CONSTANT(high, low) ((((pcg128_t)high) << 64) + low)
#define PCG_DEFAULT_MULTIPLIER_128                                             \
  PCG_128BIT_CONSTANT(2549297995355413924ULL, 4865540595714422341ULL)
#define PCG_DEFAULT_INCREMENT_128                                              \
  PCG_128BIT_CONSTANT(6364136223846793005ULL, 1442695040888963407ULL)

struct pcg_state_setseq_128 {
  pcg128_t state;
  pcg128_t inc;
};

typedef struct pcg_state_setseq_128 pcg64_random_t;
static pcg64_random_t pcg64_global; // global state

static inline void pcg_setseq_128_step_r(struct pcg_state_setseq_128 *rng) {
  rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_128 + rng->inc;
}

// call this once before calling pcg64_random_r
static inline void pcg64_seed(uint64_t seed) {
  pcg64_global.state = 0;
  __uint128_t initseq = (((__uint128_t)splitmix64_stateless(seed)) << 64) +
                        splitmix64_stateless(seed + 1);
  pcg64_global.inc = (initseq) | 1; // making sure it is odd
  pcg_setseq_128_step_r(&pcg64_global);
  pcg64_global.state += ((((__uint128_t)splitmix64_stateless(seed + 2)) << 64) +
                         splitmix64_stateless(seed + 3));
  pcg_setseq_128_step_r(&pcg64_global);
}

static inline uint64_t pcg_rotr_64(uint64_t value, unsigned int rot) {
  return (value >> rot) | (value << ((-rot) & 63));
}

static inline uint64_t pcg_output_xsl_rr_128_64(pcg128_t state) {
  return pcg_rotr_64(((uint64_t)(state >> 64u)) ^ (uint64_t)state,
                     state >> 122u);
}

static inline uint64_t
pcg_setseq_128_xsl_rr_64_random_r(struct pcg_state_setseq_128 *rng) {
  pcg_setseq_128_step_r(rng);
  return pcg_output_xsl_rr_128_64(rng->state);
}

#define pcg64_random_r pcg_setseq_128_xsl_rr_64_random_r

static inline uint64_t pcg64(void) { return pcg64_random_r(&pcg64_global); }

#endif
