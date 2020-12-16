/*
 * PRNG-style implementation of trivium (64-bit version).
 * Author: Charles Bouillaguet (charles.bouillaguet@lip6.fr).
 *
 * This version operates on 64-bit words and returns 64 pseudo-random bits.
 *
 * Trivium is a stream cipher (cryptographic-strength RNG) selected by eSTREAM
 * (part of the the EU ECRYPT project) to be part of a portfolio of secure
 * algorithms (https://www.ecrypt.eu.org/stream/).
 *
 * Trivium has been designed by Christophe De Cannière and Bart Preneel.
 * This code generates the same output as trivium's reference implementation.
 *
 * The generator takes a 64-bit seed and a 64-bit "sequence number" (this allows
 * to generate independant sequences with the same seed).
 */
#include <inttypes.h>

uint64_t trivium64_s11, trivium64_s12, trivium64_s21, trivium64_s22,
    trivium64_s31, trivium64_s32; /* global internal state */

static inline uint64_t trivium64(void) {
  uint64_t s66 = (trivium64_s12 << 62) ^ (trivium64_s11 >> 2);
  uint64_t s93 = (trivium64_s12 << 35) ^ (trivium64_s11 >> 29);
  uint64_t s162 = (trivium64_s22 << 59) ^ (trivium64_s21 >> 5);
  uint64_t s177 = (trivium64_s22 << 44) ^ (trivium64_s21 >> 20);
  uint64_t s243 = (trivium64_s32 << 62) ^ (trivium64_s31 >> 2);
  uint64_t s288 = (trivium64_s32 << 17) ^ (trivium64_s31 >> 47);
  uint64_t s91 = (trivium64_s12 << 37) ^ (trivium64_s11 >> 27);
  uint64_t s92 = (trivium64_s12 << 36) ^ (trivium64_s11 >> 28);
  uint64_t s171 = (trivium64_s22 << 50) ^ (trivium64_s21 >> 14);
  uint64_t s175 = (trivium64_s22 << 46) ^ (trivium64_s21 >> 18);
  uint64_t s176 = (trivium64_s22 << 45) ^ (trivium64_s21 >> 19);
  uint64_t s264 = (trivium64_s32 << 41) ^ (trivium64_s31 >> 23);
  uint64_t s286 = (trivium64_s32 << 19) ^ (trivium64_s31 >> 45);
  uint64_t s287 = (trivium64_s32 << 18) ^ (trivium64_s31 >> 46);
  uint64_t s69 = (trivium64_s12 << 59) ^ (trivium64_s11 >> 5);
  uint64_t t1 = s66 ^ s93; /* update */
  uint64_t t2 = s162 ^ s177;
  uint64_t t3 = s243 ^ s288;
  uint64_t z = t1 ^ t2 ^ t3;
  t1 ^= (s91 & s92) ^ s171;
  t2 ^= (s175 & s176) ^ s264;
  t3 ^= (s286 & s287) ^ s69;
  trivium64_s12 = trivium64_s11; /* rotate */
  trivium64_s11 = t3;
  trivium64_s22 = trivium64_s21;
  trivium64_s21 = t1;
  trivium64_s32 = trivium64_s31;
  trivium64_s31 = t2;
  return z;
}

void trivium64_seed(uint64_t seed, uint64_t seq) {
  trivium64_s11 = seed;
  trivium64_s12 = 0;
  trivium64_s21 = seq;
  trivium64_s22 = 0;
  trivium64_s31 = 0;
  trivium64_s32 = 0x700000000000;
  for (int i = 0; i < 18; i++) /* blank rounds */
    trivium64();
}