#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "splitmix64.h"

#define buffer_size 512
int main() {
  uint64_t seedvalue = 12345678;
  splitmix64_seed(seedvalue);
  uint64_t buffer[buffer_size];

  while (1) {
    for (int k = 0; k < buffer_size; k++)
      buffer[k] = splitmix64();
    fwrite((void *)buffer, sizeof(buffer), 1, stdout);
  }
}
