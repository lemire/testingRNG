#include "w1rand.h"

static inline void thisrng_seed(uint64_t seed) { w1rand_seed(seed); }

static inline uint64_t thisrng() { return w1rand(); }

const char *name = "w1rand";

#include "main.h"
