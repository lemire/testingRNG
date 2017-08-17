#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "TestU01.h"
#include "util64bits32bits.h"

unsigned int rng_lsb(void) { return lsb64(thisrng()); }

unsigned int rng_lsb_reverse(void) { return bytereverse32(lsb64(thisrng())); }

const int number_of_rng = 2;
unsigned int (*our_rng[number_of_rng])(void) = {rng_lsb, rng_lsb_reverse};
const char *our_name[number_of_rng] = {" lsb 32-bits ", " lsb 32-bits (reverse) "};

void printusage(const char *command) {
  printf(" %s -s : small crush", command);
  ;
  printf(" %s -c : crush", command);
  ;
  printf(" %s -b : big crush", command);
  ;
  printf(" %s : proceed until failure", command);
  ;
}
const char *success_string = "All tests were passed";
const int buffer_size = 1024 * 64;

int small_crush(unif01_Gen *gen) {
  int returnval;
  char *outputstring = malloc(buffer_size);
  memset(outputstring, 0, buffer_size);
  setbuffer(stdout, outputstring, buffer_size);

  printf("Starting small crush\n");
  fflush(NULL);

  bbattery_SmallCrush(gen);
  returnval = (strstr(outputstring, success_string) != NULL);
  fflush(NULL);
  free(outputstring);
  setlinebuf(stdout);
  return returnval;
}

int just_crush(unif01_Gen *gen) {
  int returnval;
  char *outputstring = malloc(buffer_size);
  memset(outputstring, 0, buffer_size);
  setbuffer(stdout, outputstring, buffer_size);

  printf("Starting crush\n");
  fflush(NULL);

  bbattery_Crush(gen);
  returnval = (strstr(outputstring, success_string) != NULL);
  fflush(NULL);
  free(outputstring);
  setlinebuf(stdout);
  return returnval;
}

int big_crush(unif01_Gen *gen) {
  int returnval;
  char *outputstring = malloc(buffer_size);
  memset(outputstring, 0, buffer_size);
  setbuffer(stdout, outputstring, buffer_size);

  printf("Starting big crush\n");
  fflush(NULL);

  bbattery_BigCrush(gen);
  returnval = (strstr(outputstring, success_string) != NULL);
  fflush(NULL);
  free(outputstring);
  setlinebuf(stdout);
  return returnval;
}

char* concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int main(int argc, char **argv) {
  uint64_t seedvalue = 12345678;
  thisrng_seed(seedvalue);
  printf("seed: %llu \n", (unsigned long long)seedvalue);
  unif01_Gen *gen;
  printf("%s \n", name);

  enum { SMALLCRUSH, CRUSH, BIGCRUSH, UNTILFAILURE };
  int testroutine = UNTILFAILURE;
  int c;

  while ((c = getopt(argc, argv, "csbh")) != -1)
    switch (c) {
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

  for (int z = 0; (z < number_of_rng) && returnval; z++) {
    char * tmpname = concat(name,our_name[z]);
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
  }

  if (returnval) {
    printf("Good!\n");
  } else {
    printf("Bad!\n");
  }

  return returnval == 1 ? EXIT_SUCCESS : EXIT_FAILURE;
}
