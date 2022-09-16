#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

#include "wyhash.h"

#define buffer_size 512
int main(int argc, char **argv) {
  uint64_t seedvalue = 12345678;
  wyhash64_seed(seedvalue);
  uint32_t buffer[buffer_size];
  while (1) {
      for (int k = 0; k < buffer_size; k++)
        buffer[k] = wyhash64();
      fwrite((void *)buffer, sizeof(buffer), 1, stdout);
  }
}
