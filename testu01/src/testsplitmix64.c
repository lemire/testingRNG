#include "splitmix64.h"

static inline void thisrng_seed(uint64_t seed) { splitmix64_seed(seed); }

static inline uint64_t thisrng() { return splitmix64(); }

const char *name = "splitmix64";

#include "main.h"
