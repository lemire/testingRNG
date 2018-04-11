#include "xorshift128plus.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
////
// this is the original code from Vigna's paper
// http://vigna.di.unimi.it/ftp/papers/xorshiftplus.pdf
// we want to check that it is indeed strictly equivalent
// to his later version (that we use as xorshift128plus_r).
// You can check it algebraically, but let us be super cautious.
uint64_t xorshift128plus_orig_r(xorshift128plus_key_t *key) {
  uint64_t s1 = key->part1;
  const uint64_t s0 = key->part2;
  const uint64_t result = s0 + s1;
  key->part1 = s0;
  s1 ^= s1 << 23; // a
  key->part2 = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
  return result;
}

bool are_equiv(xorshift128plus_key_t *key) {
  xorshift128plus_key_t key1 = *key;
  xorshift128plus_key_t key2 = *key;

  xorshift128plus_orig_r(&key1);// we spin this one to synchronize
  // rest should be equal

  for(size_t i = 0; i < 1024 ; i++) {
    if(xorshift128plus_orig_r(&key1) != xorshift128plus_r(&key2))
      return false;
  }
  return true;
}


int main() {
  for(size_t seed = 0; seed < 1000000; seed++) {
    xorshift128plus_key_t key;
    key.part1 = splitmix64_stateless(seed);
    key.part2 = splitmix64_stateless(seed + 1);
    if(!are_equiv(&key)) {
      printf("Bug!\n");
      return -1;
    }
  }
  printf("The two functions are equivalent.\n");
  return 0;
}
