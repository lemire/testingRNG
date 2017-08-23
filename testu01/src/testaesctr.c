#include "aesctr.h"

static inline void thisrng_seed(uint64_t seed) { aesctr_seed(seed); }

static inline uint64_t thisrng() { return aesctr(); }

const char *name = "aesctr";

#include "main.h"
