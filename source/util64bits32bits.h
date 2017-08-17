#ifndef UTIL64BITS32BITS_H
#define UTIL64BITS32BITS_H

#include <stdlib.h>
#include <stdint.h>

#ifdef _MSC_VER
// reverse the order of the bytes
static inline uint32_t bytereverse32(uint32_t value) {
  return ::_byteswap_ulong(value);
}

// reverse the order of the bytes
static inline uint64_t bytereverse64(uint64_t value) {
  return ::_byteswap_uint64(value);
}
#else

// reverse the order of the bytes
static inline uint32_t bytereverse32(uint32_t value) {
  return __builtin_bswap32(value);
}

// reverse the order of the bytes
static inline uint64_t bytereverse64(uint64_t value) {
  return __builtin_bswap64(value);
}
#endif

uint32_t lsb64(uint64_t x) { return (uint32_t)x; }

uint32_t msb64(uint64_t x) { return (uint32_t)(x >> 32); }

#endif // UTIL64BITS32BITS_H
