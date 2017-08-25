#ifndef MERSENNETWISTER_H
#define MERSENNETWISTER_H

// adapted from code by Piotr Stefaniak

#include <stdint.h>

/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.


   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static uint32_t mersennetwister_mt[N]; /* the array for the state vector  */
static unsigned int mersennetwister_mti =
    N + 1; /* mersennetwister_mti==N+1 means mersennetwister_mt[N] is not
              initialized */

/* initializes mersennetwister_mt[N] with a seed */
static inline void mersennetwister_seed(uint64_t seed) {
  mersennetwister_mt[0] = seed & 0xffffffffUL;
  for (mersennetwister_mti = 1; mersennetwister_mti < N;
       mersennetwister_mti++) {
    mersennetwister_mt[mersennetwister_mti] =
        (1812433253UL * (mersennetwister_mt[mersennetwister_mti - 1] ^
                         (mersennetwister_mt[mersennetwister_mti - 1] >> 30)) +
         mersennetwister_mti);
    /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
    /* In the previous versions, MSBs of the seed affect   */
    /* only MSBs of the array mersennetwister_mt[].                        */
    /* 2002/01/09 modified by Makoto Matsumoto             */
    mersennetwister_mt[mersennetwister_mti] &= 0xffffffffUL;
    /* for >32 bit machines */
  }
}

/* generates a random number on [0,0xffffffff]-interval */
static inline uint32_t mersennetwister(void) {
  uint32_t y;

  if (mersennetwister_mti >= N) { /* generate N words at one time */
    static uint32_t mag01[2] = {0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */
    int kk;

    for (kk = 0; kk < N - M; kk++) {
      y = (mersennetwister_mt[kk] & UPPER_MASK) |
          (mersennetwister_mt[kk + 1] & LOWER_MASK);
      mersennetwister_mt[kk] =
          mersennetwister_mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    for (; kk < N - 1; kk++) {
      y = (mersennetwister_mt[kk] & UPPER_MASK) |
          (mersennetwister_mt[kk + 1] & LOWER_MASK);
      mersennetwister_mt[kk] =
          mersennetwister_mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
    }
    y = (mersennetwister_mt[N - 1] & UPPER_MASK) |
        (mersennetwister_mt[0] & LOWER_MASK);
    mersennetwister_mt[N - 1] =
        mersennetwister_mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

    mersennetwister_mti = 0;
  }

  y = mersennetwister_mt[mersennetwister_mti++];

  /* Tempering */
  y ^= (y >> 11);
  y ^= (y << 7) & 0x9d2c5680UL;
  y ^= (y << 15) & 0xefc60000UL;
  y ^= (y >> 18);

  return y;
}

#undef N
#undef M
#undef MATRIX_A
#undef UPPER_MASK
#undef LOWER_MASK

#endif
