#include "wyrand.h"

static inline void thisrng_seed(uint64_t seed) { wyrand_seed(seed); }

static inline uint64_t thisrng() { return wyrand(); }

const char *name = "wyrand";

#include "main.h"
