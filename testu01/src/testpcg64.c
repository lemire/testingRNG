#include "pcg64.h"

static inline void thisrng_seed(uint64_t seed) { pcg64_seed(seed); }

static inline uint64_t thisrng() { return pcg64(); }

const char *name = "pcg64";

#include "main.h"
