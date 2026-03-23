#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mersennetwister.h"

#define buffer_size 512
int main() {
  uint64_t seedvalue = 12345678;
  mersennetwister_seed(seedvalue);
  uint32_t buffer[buffer_size];

  while (1) {
    for (int k = 0; k < buffer_size; k++)
      buffer[k] = mersennetwister();
    fwrite((void *)buffer, sizeof(buffer), 1, stdout);
  }
}
