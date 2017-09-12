#include "xorshift1024star.h"

static inline void thisrng_seed(uint64_t seed) { xorshift1024star_seed(seed); }

static inline uint64_t thisrng() { return xorshift1024star(); }

const char *name = "xorshift1024star";

#include "main.h"
