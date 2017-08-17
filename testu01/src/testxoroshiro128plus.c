#include "xoroshiro128plus.h"

static inline void thisrng_seed(uint64_t seed) { xoroshiro128plus_seed(seed); }

static inline uint64_t thisrng() { return xoroshiro128plus(); }

const char *name = "xoroshiro128plus";

#include "main.h"
