#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _XOPEN_SOURCE 10000
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "TestU01.h"
#include "util64bits32bits.h"

static unsigned int rng_lsb(void) { return lsb64(thisrng()); }
static unsigned int rng_lsb_reverse(void) { return bytereverse32(lsb64(thisrng())); }
static unsigned int rng_lsb_bitreverse(void) { return bitreverse32(lsb64(thisrng())); }

static unsigned int rng_msb(void) { return msb64(thisrng()); }
static unsigned int rng_msb_reverse(void) { return bytereverse32(msb64(thisrng())); }
static unsigned int rng_msb_bitreverse(void) { return bitreverse32(msb64(thisrng())); }


#define number_of_rng 6
unsigned int (*our_rng[number_of_rng])(void) = {rng_lsb, rng_lsb_reverse, rng_lsb_bitreverse, rng_msb, rng_msb_reverse, rng_msb_bitreverse};
const char *our_name[number_of_rng] = {" lsb 32-bits ",
                                       " lsb 32-bits (byte reverse) ",
                                       " lsb 32-bits (bit reverse) ",
                                       " msb 32-bits ",
                                       " msb 32-bits (byte reverse) ",
                                       " msb 32-bits (bit reverse) ",};

void printusage(const char *command) {
  printf(" %s -s : small crush", command);
  ;
  printf(" %s -c : crush", command);
  ;
  printf(" %s -b : big crush", command);
  ;
  printf(" %s -l : linear complexity", command);
  ;

  printf(" %s : proceed until failure", command);
  ;
  printf(" The -r flag reverses the bytes.");
  ;
  printf(" The -R and the -z flags reverse the bits (they are synonymous).");
  ;
  printf(" The -H flag select to most signficant 32 bits (as opposed to the least significant).");
  ;
  printf(" The -S flag allows you to pass a seed (e.g., -S 42132).");
  ;

}

char *concat(const char *s1, const char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

int main(int argc, char **argv) {
  uint64_t seedvalue = 12345678;
  unif01_Gen *gen;
  int z = 0;// selects the folding method

  enum { SMALLCRUSH, CRUSH, BIGCRUSH, LINEARCOMP};
  int use_msb = 0;
  int testroutine = SMALLCRUSH;
  int c;

  while ((c = getopt(argc, argv, "csbhrzRHlS:")) != -1)
    switch (c) {
    case 'l':
      testroutine = LINEARCOMP;
      break;
    case 'S':
      seedvalue = atoi(optarg);
      break;
    case 'r':
      z = 1;
      break;
    case 'R':
      z = 2;
      break;
    case 'z':
      z = 2;
      break;
    case 's':
      testroutine = SMALLCRUSH;
      break;
    case 'c':
      testroutine = CRUSH;
      break;
    case 'b':
      testroutine = BIGCRUSH;
      break;
    case 'H':
      use_msb = 1;
      break;
    case 'h':
      printusage(argv[0]);
      return 0;
    default:
      abort();
    }
  printf("==seed: %llu \n", (unsigned long long)seedvalue);
  thisrng_seed(seedvalue);
  printf("==%s \n", name);
  if(use_msb) z += 3;
  char *tmpname = concat(name, our_name[z]);
  gen = unif01_CreateExternGenBits(tmpname, our_rng[z]);

  switch (testroutine) {
  case SMALLCRUSH:
    bbattery_SmallCrush(gen);
    break;
  case CRUSH:
    bbattery_Crush(gen);
    break;
  case BIGCRUSH:
    bbattery_BigCrush(gen);
    break;
  case LINEARCOMP:
    // from O'Neill's
    {
    scomp_Res* res = scomp_CreateRes();
    swrite_Basic = TRUE;
    int size_array[] = {5000, 25000, 50000};
    for (size_t k = 0; k < sizeof(size_array)/sizeof(int); k++) {
        scomp_LinearComp(gen, res, 1, size_array[k], 0, 1);
    }
    scomp_DeleteRes(res);
    fflush(stdout);
    }
    break;

  default:
    abort();
  }

  unif01_DeleteExternGenBits(gen);
  free(tmpname);


  return EXIT_SUCCESS;
}
