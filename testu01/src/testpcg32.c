#include "pcg32.h"

static inline void thisrng_seed(uint64_t seed) { pcg32_seed(seed); }

static inline uint64_t thisrng() { return pcg32(); }

const char *name = "pcg32";

#include "main.h"
