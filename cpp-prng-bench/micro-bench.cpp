/*
 * Benchmarking Code.
 *     To use, define RNG to be the generator (e.g., with
 *     -DRNG_TYPE=std::mt19937) and INCLUDE_FILE to any file that
 *     should be included.  Optionally you can define FASTER to be a
 *     number greater than 1.  Compile with USE_TSC to get cycle
 *     counts.
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-2017 Melissa E. O'Neill
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <unistd.h>

#ifdef INCLUDE_FILE
#include INCLUDE_FILE
#endif

#define really_stringify(x) #x
#define stringify(x) really_stringify(x)

#ifndef GENERATE
#define GENERATE(rng) rng()
#endif

#ifndef RNG_TYPE
    #define RNG_TYPE std::default_random_engine
#endif
#ifndef FASTER
    constexpr auto FASTER = 1;
#endif
typedef RNG_TYPE rng;

void doit()
{
    using namespace std::chrono;
    using namespace std;

    rng generator(0xcafef00dU);
    rng::result_type count = 0;
#if USE_TSC
    constexpr auto OUTER_ROUNDS = 10000;
    constexpr auto INNER_ROUNDS = 100000 / FASTER;
    size_t cycles[OUTER_ROUNDS];
#else
    constexpr auto OUTER_ROUNDS = 30;
    constexpr auto INNER_ROUNDS = 100000000 / FASTER;
    double secs[OUTER_ROUNDS];
#endif

    for(int j = 0; j < OUTER_ROUNDS; ++j) {
    #if USE_TSC
        size_t start = __builtin_ia32_rdtsc();
    #else
        auto start = high_resolution_clock::now();
    #endif

        for (int i = 0; i < INNER_ROUNDS; ++i) {
            count += 1;
            asm("# Do nothing" : "+r" (count));
        }

    #if USE_TSC
        size_t finish = __builtin_ia32_rdtsc();
        cycles[j] = finish - start;
    #else
        auto finish = high_resolution_clock::now();
        secs[j] = duration_cast<duration<double>>(finish - start).count();
    #endif
    }


#if USE_TSC
    sort(begin(cycles), end(cycles));
    auto cycles0 = cycles[OUTER_ROUNDS/10];
#else
    sort(begin(secs), end(secs));
    auto secs0 = secs[OUTER_ROUNDS/4];
#endif


    for(int j = 0; j < OUTER_ROUNDS; ++j) {
    #if USE_TSC
        size_t start = __builtin_ia32_rdtsc();
    #else
        auto start = high_resolution_clock::now();
    #endif

        for (int i = 0; i < INNER_ROUNDS; ++i) {
            count += GENERATE(generator);
        }

    #if USE_TSC
        size_t finish = __builtin_ia32_rdtsc();
        cycles[j] = finish - start;
    #else
        auto finish = high_resolution_clock::now();
        secs[j] = duration_cast<duration<double>>(finish - start).count();
    #endif
    }

    auto range = (generator.max() - generator.min() + 1);
    double bits = range ? log2(double(range)) : double(sizeof(range)*8);
    if (sizeof(range)*8 == 128)
        bits = 128;
    printf("# " stringify(RNG_TYPE) ": size = %zu, bits = %.10f, ",
           sizeof(rng), bits);
    int cbits = ceil(bits);
    if (range > 0 && bits != cbits) {
        printf("range = 2^%d - %zu\n", cbits, size_t((1ULL << cbits) - range));
    } else {
        printf("range = 2^%d\n", cbits);
    }

#if USE_TSC
    sort(begin(cycles), end(cycles));
    auto cycles1 = cycles[OUTER_ROUNDS/10];
    auto cycles_delta = cycles1 - cycles0;

    printf("cycles: %g\n", cycles_delta/double(INNER_ROUNDS));
    printf("\tcount = %lu\n", size_t(count >> 1));
    printf("\ttotals: %zu\t%zu\t%zu\n", cycles_delta, cycles0, cycles1);
//    printf("time = %zu cycles, %.2f bits/cycle, %.2f cycles/32bit\n", cycles,
//           double(INNER_ROUNDS)*bits/double(cycles),
//           double(cycles)/(double(INNER_ROUNDS)*bits/32.0));

#else
    sort(begin(secs), end(secs));
    auto secs1 = secs[OUTER_ROUNDS/10];
    auto seconds = secs1 - secs0;

    printf("\tcount = %lu\tsizeof(count) = %zu\n", size_t(count >> 1), sizeof(count));
    printf("time = %f seconds, %.2f Gb/s, %.2f ns/32bit\n", seconds,
           double(INNER_ROUNDS)*bits/(1024.0*1024.0*1024.0)/seconds,
           seconds/(double(INNER_ROUNDS)*bits/32.0)*1e9);
#endif


}

int main()
{
    usleep(1000);
    doit();
}

