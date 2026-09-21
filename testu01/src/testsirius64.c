#include "sirius64.h"

static inline void thisrng_seed(uint64_t seed) { sirius64_seed(seed); }

static inline uint64_t thisrng() { return sirius64(); }

const char *name = "sirius64";

#include "main.h"
