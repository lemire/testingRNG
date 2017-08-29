#include "mersennetwister.h"

static inline void thisrng_seed(uint64_t seed) { mersennetwister_seed(seed); }

static inline uint32_t thisrng() { return mersennetwister(); }

const char *name = "mersennetwister";

#include "main.h"
