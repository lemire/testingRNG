/***
* Daniel Baker has adapted this code to C++:
* https://github.com/dnbaker/hll/blob/master/aesctr.h
* He reports that UNROLL_COUNT of 8 (rather than 4) is measurably
* but not enormously faster.
***/

#ifndef AESCTR_H
#define AESCTR_H
#ifdef __AES__
// contributed by Samuel Neves

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define AESCTR_UNROLL 4
#define AESCTR_ROUNDS 10

typedef struct {
  uint8_t state[16 * AESCTR_UNROLL];
  __m128i ctr[AESCTR_UNROLL];
  __m128i seed[AESCTR_ROUNDS + 1];
  size_t offset;
} aesctr_state;

#define AES_ROUND(rcon, index)                                                 \
  do {                                                                         \
    __m128i k2 = _mm_aeskeygenassist_si128(k, rcon);                           \
    k = _mm_xor_si128(k, _mm_slli_si128(k, 4));                                \
    k = _mm_xor_si128(k, _mm_slli_si128(k, 4));                                \
    k = _mm_xor_si128(k, _mm_slli_si128(k, 4));                                \
    k = _mm_xor_si128(k, _mm_shuffle_epi32(k2, _MM_SHUFFLE(3, 3, 3, 3)));      \
    state->seed[index] = k;                                                    \
  } while (0)

static inline void aesctr_seed_r(aesctr_state *state, uint64_t seed) {
  /*static const uint8_t rcon[] = {
      0x8d, 0x01, 0x02, 0x04,
      0x08, 0x10, 0x20, 0x40,
      0x80, 0x1b, 0x36
  };*/
  __m128i k = _mm_set_epi64x(0, seed);
  state->seed[0] = k;
  // D. Lemire manually unrolled following loop since _mm_aeskeygenassist_si128
  // requires immediates

  /*for(int i = 1; i <= AESCTR_ROUNDS; ++i)
  {
      __m128i k2 = _mm_aeskeygenassist_si128(k, rcon[i]);
      k = _mm_xor_si128(k, _mm_slli_si128(k, 4));
      k = _mm_xor_si128(k, _mm_slli_si128(k, 4));
      k = _mm_xor_si128(k, _mm_slli_si128(k, 4));
      k = _mm_xor_si128(k, _mm_shuffle_epi32(k2, _MM_SHUFFLE(3,3,3,3)));
      state->seed[i] = k;
  }*/
  AES_ROUND(0x01, 1);
  AES_ROUND(0x02, 2);
  AES_ROUND(0x04, 3);
  AES_ROUND(0x08, 4);
  AES_ROUND(0x10, 5);
  AES_ROUND(0x20, 6);
  AES_ROUND(0x40, 7);
  AES_ROUND(0x80, 8);
  AES_ROUND(0x1b, 9);
  AES_ROUND(0x36, 10);

  for (int i = 0; i < AESCTR_UNROLL; ++i) {
    state->ctr[i] = _mm_set_epi64x(0, i);
  }
  state->offset = 16 * AESCTR_UNROLL;
}

#undef AES_ROUND

static inline uint64_t aesctr_r(aesctr_state *state) {
  if (__builtin_expect(state->offset >= 16 * AESCTR_UNROLL, 0)) {
    __m128i work[AESCTR_UNROLL];
    for (int i = 0; i < AESCTR_UNROLL; ++i) {
      work[i] = _mm_xor_si128(state->ctr[i], state->seed[0]);
    }
    for (int r = 1; r <= AESCTR_ROUNDS - 1; ++r) {
      const __m128i subkey = state->seed[r];
      for (int i = 0; i < AESCTR_UNROLL; ++i) {
        work[i] = _mm_aesenc_si128(work[i], subkey);
      }
    }
    for (int i = 0; i < AESCTR_UNROLL; ++i) {
      state->ctr[i] =
          _mm_add_epi64(state->ctr[i], _mm_set_epi64x(0, AESCTR_UNROLL));
      _mm_storeu_si128(
          (__m128i *)&state->state[16 * i],
          _mm_aesenclast_si128(work[i], state->seed[AESCTR_ROUNDS]));
    }
    state->offset = 0;
  }
  uint64_t output = 0;
  memcpy(&output, &state->state[state->offset], sizeof(output));
  state->offset += sizeof(output);
  return output;
}

static aesctr_state g_aesctr_state;

static inline void aesctr_seed(uint64_t seed) {
  aesctr_seed_r(&g_aesctr_state, seed);
}

static inline uint64_t aesctr() { return aesctr_r(&g_aesctr_state); }

#undef AESCTR_UNROLL
#undef AESCTR_ROUNDS
#endif // __AES__
#endif
