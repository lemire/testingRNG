/*
 * PRNG-style implementation of trivium.
 * Author: Charles Bouillaguet (charles.bouillaguet@lip6.fr).
 *
 * This version operates on 32-bit words and returns 32 pseudo-random bits.
 *
 * Trivium is a stream cipher (cryptographic-strength RNG) selected by eSTREAM
 * (part of the the EU ECRYPT project) to be part of a portfolio of secure
 * algorithms (https://www.ecrypt.eu.org/stream/).
 *
 * Trivium has been designed by Christophe De Cannière and Bart Preneel.
 *
 * This code generates the same output as trivium's reference implementation.
 *
 * The generator takes a 64-bit seed and a 64-bit "sequence number" (this allows
 * to generate independant sequences with the same seed). */
#include <inttypes.h>

uint32_t trivium32_s11, trivium32_s12, trivium32_s13, trivium32_s21,
    trivium32_s22, trivium32_s23, trivium32_s31, trivium32_s32, trivium32_s33,
    trivium32_s34; /* global internal state */

static inline uint32_t trivium32(void) {
  uint32_t s66 = (trivium32_s13 << 30) ^ (trivium32_s12 >> 2);
  uint32_t s93 = (trivium32_s13 << 3) ^ (trivium32_s12 >> 29);
  uint32_t s162 = (trivium32_s23 << 27) ^ (trivium32_s22 >> 5);
  uint32_t s177 = (trivium32_s23 << 12) ^ (trivium32_s22 >> 20);
  uint32_t s243 = (trivium32_s33 << 30) ^ (trivium32_s32 >> 2);
  uint32_t s288 = (trivium32_s34 << 17) ^ (trivium32_s33 >> 15);
  uint32_t s91 = (trivium32_s13 << 5) ^ (trivium32_s12 >> 27);
  uint32_t s92 = (trivium32_s13 << 4) ^ (trivium32_s12 >> 28);
  uint32_t s171 = (trivium32_s23 << 18) ^ (trivium32_s22 >> 14);
  uint32_t s175 = (trivium32_s23 << 14) ^ (trivium32_s22 >> 18);
  uint32_t s176 = (trivium32_s23 << 13) ^ (trivium32_s22 >> 19);
  uint32_t s264 = (trivium32_s33 << 9) ^ (trivium32_s32 >> 23);
  uint32_t s286 = (trivium32_s34 << 19) ^ (trivium32_s33 >> 13);
  uint32_t s287 = (trivium32_s34 << 18) ^ (trivium32_s33 >> 14);
  uint32_t s69 = (trivium32_s13 << 27) ^ (trivium32_s12 >> 5);
  uint32_t t1 = s66 ^ s93; /* update */
  uint32_t t2 = s162 ^ s177;
  uint32_t t3 = s243 ^ s288;
  uint32_t z = t1 ^ t2 ^ t3;
  t1 ^= (s91 & s92) ^ s171;
  t2 ^= (s175 & s176) ^ s264;
  t3 ^= (s286 & s287) ^ s69;
  trivium32_s13 = trivium32_s12; /* rotate */
  trivium32_s12 = trivium32_s11;
  trivium32_s11 = t3;
  trivium32_s23 = trivium32_s22;
  trivium32_s22 = trivium32_s21;
  trivium32_s21 = t1;
  trivium32_s34 = trivium32_s33;
  trivium32_s33 = trivium32_s32;
  trivium32_s32 = trivium32_s31;
  trivium32_s31 = t2;
  return z;
}

void trivium32_seed(uint64_t seed, uint64_t seq) {
  trivium32_s11 = seed;
  trivium32_s12 = seed >> 32;
  trivium32_s13 = 0;
  trivium32_s21 = seq;
  trivium32_s22 = seq >> 32;
  trivium32_s23 = 0;
  trivium32_s31 = 0;
  trivium32_s32 = 0;
  trivium32_s33 = 0;
  trivium32_s34 = 0x7000;
  for (int i = 0; i < 36; i++) /* blank rounds */
    trivium32();
}