#include "lehmer64.h"

static inline void thisrng_seed(uint64_t seed) { lehmer64_seed(seed); }

static inline uint64_t thisrng() { return lehmer64(); }

const char *name = "lehmer64";

#include "main.h"
