#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>

#include "widynski.h"

#define buffer_size 512
int main(int argc, char **argv) {
  int c;
  while ((c = getopt(argc, argv, "")) != -1)
    switch (c) {
    default:
      abort();
    }
  uint64_t seedvalue = 12345678;
  widynski_seed(seedvalue);
  uint32_t buffer[buffer_size];
  while (1) {
      for (int k = 0; k < buffer_size; k++)
        buffer[k] = widynski();
      fwrite((void *)buffer, sizeof(buffer), 1, stdout);
  }
}
