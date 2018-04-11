#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

#include "xorshift1024plus.h"

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
  xorshift1024plus_seed(seedvalue);
  if (castingtohigh32) {
    uint32_t buffer[buffer_size];
    while (1) {
      for (int k = 0; k < buffer_size; k++)
        buffer[k] = (xorshift1024plus() >> 32);
      fwrite((void *)buffer, sizeof(buffer), 1, stdout);
    }
  } else {
    uint64_t buffer[buffer_size];
    while (1) {
      for (int k = 0; k < buffer_size; k++)
        buffer[k] = xorshift1024plus();
      fwrite((void *)buffer, sizeof(buffer), 1, stdout);
    }
  }
}
