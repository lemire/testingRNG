#ifndef WIDYNSKI_H
#define WIDYNSKI_H

#include <stdint.h>
// based on https://arxiv.org/pdf/1704.00358.pdf
static uint64_t g_widynski_x;
static uint64_t g_widynski_w;
static uint64_t g_widynski_s;

static inline void widynski_seed(uint64_t seed) {
  g_widynski_w = 0;
  g_widynski_x = 0;
  g_widynski_s = seed;
  g_widynski_s |= 1;
  if ((g_widynski_s >> 32) == 0)
    g_widynski_s = g_widynski_s | (g_widynski_s << 32);
}

static inline uint32_t widynski() {
  g_widynski_x *= g_widynski_x;
  g_widynski_x += (g_widynski_w += g_widynski_s);
  g_widynski_x = (g_widynski_x >> 32) | (g_widynski_x << 32);
  return g_widynski_x;
}

#endif
