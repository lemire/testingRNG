#include "wyhash.h"

static inline void thisrng_seed(uint64_t seed) { wyhash64_seed(seed); }

static inline uint64_t thisrng() { return wyhash64(); }

const char *name = "wyhash64";

#include "main.h"
