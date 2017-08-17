#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _XOPEN_SOURCE 10000
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "ioredirect.h"
#include "TestU01.h"
#include "util64bits32bits.h"

unsigned int rng_lsb(void) { return lsb64(thisrng()); }

unsigned int rng_lsb_reverse(void) { return bytereverse32(lsb64(thisrng())); }
unsigned int rng_lsb_bitreverse(void) { return bitreverse32(lsb64(thisrng())); }

#define number_of_rng 3
unsigned int (*our_rng[number_of_rng])(void) = {rng_lsb, rng_lsb_reverse, rng_lsb_bitreverse};
const char *our_name[number_of_rng] = {" lsb 32-bits ",
                                       " lsb 32-bits (byte reverse) ",
                                       " lsb 32-bits (bit reverse) ",};

void printusage(const char *command) {
  printf(" %s -s : small crush", command);
  ;
  printf(" %s -c : crush", command);
  ;
  printf(" %s -b : big crush", command);
  ;
  printf(" %s : proceed until failure", command);
  ;
  printf(" The -r flag reverses the bytes.");
  ;
  printf(" The -R flag reverses the bits.");
  ;
}
const char *success_string = "All tests were passed";

int small_crush(unif01_Gen *gen) {
  static char template[] = "smallcrushXXXXXXXXX";
  char buffer[24];
  strcpy(buffer, template);
  mktemp(buffer);
  FILE *fp = fopen(buffer, "w");
  setvbuf( fp, (char *)NULL, _IONBF, 0 ); // no buffering please!
  printf("==Logging temporarily to %s \n", buffer);
  SwapIOB(stdout, fp);
  bbattery_SmallCrush(gen);
  SwapIOB(fp, stdout);
  fclose(fp);
  int ret = printAndSeekSubstring(buffer, success_string);
  // intentionally, we want the file to stick around till we are done processing
  // it (in case of a crash)
  unlink(buffer);
  return ret;
}

int just_crush(unif01_Gen *gen) {
  static char template[] = "crushXXXXXXXXX";
  char buffer[24];
  strcpy(buffer, template);
  mktemp(buffer);
  FILE *fp = fopen(buffer, "w");
  setvbuf( fp, (char *)NULL, _IONBF, 0 ); // no buffering please!
  printf("==Logging temporarily to %s \n", buffer);
  SwapIOB(stdout, fp);
  bbattery_Crush(gen);
  SwapIOB(fp, stdout);
  fclose(fp);
  int ret = printAndSeekSubstring(buffer, success_string);
  // intentionally, we want the file to stick around till we are done processing
  // it (in case of a crash)
  unlink(buffer);
  return ret;
}

int big_crush(unif01_Gen *gen) {
  static char template[] = "bigcrushXXXXXXXXX";
  char buffer[24];
  strcpy(buffer, template);
  mktemp(buffer);
  FILE *fp = fopen(buffer, "w");
  setvbuf( fp, (char *)NULL, _IONBF, 0 ); // no buffering please!
  printf("==Logging temporarily to %s \n", buffer);
  SwapIOB(stdout, fp);
  bbattery_BigCrush(gen);
  SwapIOB(fp, stdout);
  fclose(fp);
  int ret = printAndSeekSubstring(buffer, success_string);
  // intentionally, we want the file to stick around till we are done processing
  // it (in case of a crash)
  unlink(buffer);
  return ret;
}

char *concat(const char *s1, const char *s2) {
  char *result = malloc(strlen(s1) + strlen(s2) + 1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

int main(int argc, char **argv) {
  uint64_t seedvalue = 12345678;
  thisrng_seed(seedvalue);
  printf("==seed: %llu \n", (unsigned long long)seedvalue);
  unif01_Gen *gen;
  printf("==%s \n", name);
  int z = 0;

  enum { SMALLCRUSH, CRUSH, BIGCRUSH, UNTILFAILURE };
  int testroutine = UNTILFAILURE;
  int c;

  while ((c = getopt(argc, argv, "csbhrR")) != -1)
    switch (c) {
    case 'r':
      z = 1;
      break;
    case 'R':
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
    case 'h':
      printusage(argv[0]);
      return 0;
    default:
      abort();
    }
  int returnval = 1;

  char *tmpname = concat(name, our_name[z]);
  gen = unif01_CreateExternGenBits(tmpname, our_rng[z]);

  switch (testroutine) {
  case UNTILFAILURE:
    if (returnval)
      returnval = small_crush(gen);
    if (returnval)
      returnval = just_crush(gen);
    if (returnval)
      returnval = big_crush(gen);
    break;
  case SMALLCRUSH:
    returnval = small_crush(gen);
    break;
  case CRUSH:
    returnval = just_crush(gen);
    break;
  case BIGCRUSH:
    returnval = big_crush(gen);
    break;
  default:
    abort();
  }

  unif01_DeleteExternGenBits(gen);
  free(tmpname);

  if (returnval) {
    printf("==Good!\n");
  } else {
    printf("==Bad!\n");
  }

  return returnval == 1 ? EXIT_SUCCESS : EXIT_FAILURE;
}
