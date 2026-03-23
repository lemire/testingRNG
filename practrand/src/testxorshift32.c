#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "xorshift32.h"

#define buffer_size 512
int main() {
  uint64_t seedvalue = 12345678;
  xorshift32_seed(seedvalue);
  uint32_t buffer[buffer_size];

  while (1) {
    for (int k = 0; k < buffer_size; k++)
      buffer[k] = xorshift32();
    fwrite((void *)buffer, sizeof(buffer), 1, stdout);
  }
}
