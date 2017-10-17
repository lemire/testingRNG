#include "xorshift1024plus.h"

static inline void thisrng_seed(uint64_t seed) { xorshift1024plus_seed(seed); }

static inline uint64_t thisrng() { return xorshift1024plus(); }

const char *name = "xorshift1024plus";

#include "main.h"
