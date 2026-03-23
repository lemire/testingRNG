#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <vector>
#include <format>
#include <iostream>

#include "counters/bench.h"

#include "xorshift32.h"
#include "pcg32.h"
#include "xorshift128plus.h"
#include "xorshift1024star.h"
#include "xorshift1024plus.h"
#include "xoroshiro128plus.h"
#include "splitmix64.h"
#include "splitmix63.h"
#include "pcg64.h"
#include "lehmer64.h"
#include "mersennetwister.h"
#include "mitchellmoore.h"
#include "xorshift-k4.h"
#include "xorshift-k5.h"
#include "widynski.h"
#include "wyhash.h"
#include "wyrand.h"
#include "w1rand.h"
#include "trivium32.h"
#include "trivium64.h"
#include "CG128.h"
#include "CG64.h"
#include "CG128-64.h"
#include "jenkinssmall.h"
#ifdef __AES__
#include "aesctr.h"
#include "aesdragontamer.h"
#endif

typedef uint32_t (*rand32fnc)(void);
typedef uint64_t (*rand64fnc)(void);
typedef __uint128_t (*rand128fnc)(void);

static uint32_t rand_wrapper() { return (uint32_t)rand(); }

struct gen32_entry { rand32fnc fn; const char *name; };
struct gen64_entry { rand64fnc fn; const char *name; };
struct gen128_entry { rand128fnc fn; const char *name; };

gen32_entry generators32[] = {
    {trivium32, "trivium"},
    {xorshift_k4, "xorshift_k4"},
    {xorshift_k5, "xorshift_k5"},
    {mersennetwister, "mersennetwister"},
    {mitchellmoore, "mitchellmoore"},
    {widynski, "widynski"},
    {xorshift32, "xorshift32"},
    {pcg32, "pcg32"},
    {rand_wrapper, "rand"},
};

gen64_entry generators64[] = {
    {trivium64, "trivium64"},
#ifdef __AES__
    {aesdragontamer, "aesdragontamer"},
    {aesctr, "aesctr"},
#endif
    {lehmer64, "lehmer64"},
    {xorshift128plus, "xorshift128plus"},
    {xoroshiro128plus, "xoroshiro128plus"},
    {splitmix64, "splitmix64"},
    {splitmix63, "splitmix63"},
    {pcg64, "pcg64"},
    {xorshift1024star, "xorshift1024star"},
    {xorshift1024plus, "xorshift1024plus"},
    {wyhash64, "wyhash64"},
    {wyrand, "wyrand"},
    {w1rand, "w1rand"},
    {jenkinssmall, "jenkinssmall"},
    {CG64, "CG64"},
};

gen128_entry generators128[] = {
    {CG128, "CG128"},
    {CG128_64, "CG128_64"},
};

void populate32(rand32fnc f, uint32_t *answer, size_t size) {
  for (size_t i = size; i != 0; i--) {
    answer[size - i] = f();
  }
}

void populate64(rand64fnc f, uint64_t *answer, size_t size) {
  for (size_t i = size; i != 0; i--) {
    answer[size - i] = f();
  }
}

void populate128(rand128fnc f, __uint128_t *answer, size_t size) {
  for (size_t i = size; i != 0; i--) {
    answer[size - i] = f();
  }
}

void pretty_print(const char *name, size_t bytes,
                  counters::event_aggregate agg) {
  std::cout << std::format("{:<40} : ", name);
  std::cout << std::format(" {:5.2f} ns/byte ", agg.fastest_elapsed_ns() / double(bytes));
  std::cout << std::format(" {:5.2f} GB/s ",
             double(bytes) / agg.fastest_elapsed_ns());
  if (counters::has_performance_counters()) {
    std::cout << std::format(" {:5.2f} GHz ", agg.cycles() / double(agg.elapsed_ns()));
    std::cout << std::format(" {:5.2f} c/b ", agg.fastest_cycles() / double(bytes));
    std::cout << std::format(" {:5.2f} i/b ", agg.fastest_instructions() / double(bytes));
    std::cout << std::format(" {:5.2f} i/c ",
               agg.fastest_instructions() / double(agg.fastest_cycles()));
  }
  std::cout << "\n";
}

void run_benchmark(int size) {
  std::cout << std::format("Generating {} bytes of random numbers\n", size);
  std::cout << "Time reported per byte.\n";
  std::cout << std::format("We store values to an array of size = {} kB.\n\n", size / 1024);

  void *buf = malloc(size);
  assert(buf);
  assert(size % 8 == 0);

  std::cout << "32-bit generators:\n";
  for (auto &g : generators32) {
    auto fn = g.fn;
    auto results = counters::bench([&]() {
      populate32(fn, (uint32_t *)buf, size / sizeof(uint32_t));
    });
    pretty_print(g.name, size, results);
  }

  std::cout << "\n64-bit generators:\n";
  for (auto &g : generators64) {
    auto fn = g.fn;
    auto results = counters::bench([&]() {
      populate64(fn, (uint64_t *)buf, size / sizeof(uint64_t));
    });
    pretty_print(g.name, size, results);
  }

  std::cout << "\n128-bit generators:\n";
  for (auto &g : generators128) {
    auto fn = g.fn;
    auto results = counters::bench([&]() {
      populate128(fn, (__uint128_t *)buf, size / sizeof(__uint128_t));
    });
    pretty_print(g.name, size, results);
  }

  free(buf);
  std::cout << "\n";
}

int main() {
  std::cout << "\n";
  if (counters::has_performance_counters()) {
    std::cout << "Performance counters are available.\n";
  } else {
    std::cout << "Performance counters are unavailable. Only timing will be reported.\n";
    std::cout << "You may be able to get counters by running as root (sudo).\n";
  }
  run_benchmark(4096);
  return EXIT_SUCCESS;
}
