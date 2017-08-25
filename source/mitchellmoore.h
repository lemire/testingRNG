#ifndef MITCHELLMOORE_H
#define MITCHELLMOORE_H

// adapted from code by Piotr Stefaniak

#include <stdint.h>

/* Mitchell-Moore algorithm from
 * "The Art of Computer Programming, Volume II"
 * by Donald E. Knuth
 *
 * Improvements based on
 * "Uniform Random Number Generators for Vector and Parallel Computers"
 * by Richard P. Brent */

#define R 250U
#define S 200U
#define T 103U
#define U 50U

static uint32_t mitchellmoore_sequence[R];
static unsigned int mitchellmoore_a = R, mitchellmoore_b = S,
                    mitchellmoore_c = T, mitchellmoore_d = U;

static inline void mitchellmoore_seed(uint64_t seed) {
  unsigned int i;

  for (i = 0; i < R * 2; i++)
    mitchellmoore_sequence[i % R] = seed = (1664525 * seed + 1013904223);

  mitchellmoore_sequence[0] <<= 1;
  mitchellmoore_sequence[1] |= 1;

  return;
}

static inline uint32_t mitchellmoore(void) {
  return mitchellmoore_sequence[++mitchellmoore_a % R] +=
         mitchellmoore_sequence[++mitchellmoore_b % R] +=
         mitchellmoore_sequence[++mitchellmoore_c % R] +=
         mitchellmoore_sequence[++mitchellmoore_d % R];
}

#undef R
#undef S
#undef T
#undef U
#endif
