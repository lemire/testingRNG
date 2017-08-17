#include "xorshift32.h"

static inline void thisrng_seed(uint64_t seed) { xorshift32_seed(seed); }

static inline uint64_t thisrng() { return xorshift32(); }

const char *name = "splitmix64";

#include "main.h"
