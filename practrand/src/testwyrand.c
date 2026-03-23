#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

#include "wyrand.h"

#define buffer_size 512
int main(int argc, char **argv) {
  int c;
  int castingtohigh32 = 0;
  while ((c = getopt(argc, argv, "H")) != -1)
    switch (c) {
    case 'H':
      castingtohigh32 = 1;
      break;
    default:
      abort();
    }
  uint64_t seedvalue = 12345678;
  wyrand_seed(seedvalue);
  if (castingtohigh32) {
    uint32_t buffer[buffer_size];
    while (1) {
      for (int k = 0; k < buffer_size; k++)
        buffer[k] = (wyrand() >> 32);
      fwrite((void *)buffer, sizeof(buffer), 1, stdout);
    }
  } else {
    uint32_t buffer[buffer_size];
    while (1) {
      for (int k = 0; k < buffer_size; k++)
        buffer[k] = wyrand();
      fwrite((void *)buffer, sizeof(buffer), 1, stdout);
    }
  }
}
