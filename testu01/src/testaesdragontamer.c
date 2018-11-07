#ifdef __AES__ 
#include "aesdragontamer.h"

static inline void thisrng_seed(uint64_t seed) { aesdragontamer_seed(seed); }

static inline uint64_t thisrng() { return aesdragontamer(); }

const char *name = "aesdragontamer";

#include "main.h"
#endif