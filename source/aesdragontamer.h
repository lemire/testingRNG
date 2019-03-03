/***
* Code from Percy "Dragontamer" Tiglao
***/

#ifndef AESDRAGONTAMER_H
#define AESDRAGONTAMER_H
#ifdef __AES__

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "splitmix64.h"

typedef struct {
  __m128i state;
  __m128i increment;
  uint64_t buffer[4];
  size_t offset;
} aesdragontamer_state;

static inline void aesdragontamer_tobuffer_r(aesdragontamer_state *state) {
  state->state = _mm_add_epi64(state->state, state->increment);
  __m128i penultimate = _mm_aesenc_si128(state->state, state->increment);
  __m128i penultimate1 = _mm_aesenc_si128(penultimate, state->increment);
  __m128i penultimate2 = _mm_aesdec_si128(penultimate, state->increment);

  _mm_storeu_si128((__m128i *)state->buffer, penultimate1); // stores 2 uint64_t
  _mm_storeu_si128((__m128i *)state->buffer + 1,
                   penultimate2); // stores 2 uint64_t
  state->offset = 0;
}

static inline void aesdragontamer_seed_r(aesdragontamer_state *state,
                                         uint64_t seed) {
  state->increment =
      _mm_set_epi8(0x2f, 0x2b, 0x29, 0x25, 0x1f, 0x1d, 0x17, 0x13, 0x11, 0x0D,
                   0x0B, 0x07, 0x05, 0x03, 0x02, 0x01);
  state->offset = 0;
  state->state = _mm_set_epi64x(splitmix64_stateless(seed),
                                splitmix64_stateless(seed + 1));
  aesdragontamer_tobuffer_r(state);
}

static inline uint64_t aesdragontamer_r(aesdragontamer_state *state) {
  if (state->offset == 4)
    aesdragontamer_tobuffer_r(
        state); // every 4 calls, we generate 4 new uint64_t
  return state->buffer[state->offset++];
}

static aesdragontamer_state g_aesdragontamer_state;

static inline void aesdragontamer_seed(uint64_t seed) {
  aesdragontamer_seed_r(&g_aesdragontamer_state, seed);
}

static inline uint64_t aesdragontamer() {
  return aesdragontamer_r(&g_aesdragontamer_state);
}

#endif // __AES__
#endif
