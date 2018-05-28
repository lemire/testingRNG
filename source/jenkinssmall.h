#ifndef JENKINSSMALL_H
#define JENKINSSMALL_H

// http://burtleburtle.net/bob/rand/smallprng.html

#include "splitmix64.h"
#include <stdint.h>

typedef struct jenkinssmall_ranctx {
  uint64_t a;
  uint64_t b;
  uint64_t c;
  uint64_t d;
} jenkinssmall_ranctx;

static jenkinssmall_ranctx jenkinssmall_y;

void jenkinssmall_init(uint64_t seed) {
  uint64_t i;
  jenkinssmall_y.a = 0xf1ea5eed,
  jenkinssmall_y.b = jenkinssmall_y.c = jenkinssmall_y.d = seed;
  for (i = 0; i < 20; ++i) {
    (void)ranval();
  }
}

#define rot(x, k) (((x) << (k)) | ((x) >> (64 - (k))))

uint64_t jenkinssmall(void) {
  uint64_t e = jenkinssmall_y.a - rot(jenkinssmall_y.b, 7);
  jenkinssmall_y.a = jenkinssmall_y.b ^ rot(jenkinssmall_y.c, 13);
  jenkinssmall_y.b = jenkinssmall_y.c + rot(jenkinssmall_y.d, 37);
  jenkinssmall_y.c = jenkinssmall_y.d + e;
  jenkinssmall_y.d = e + jenkinssmall_y.a;
  return jenkinssmall_y.d;
}

#undef rot

#endif
