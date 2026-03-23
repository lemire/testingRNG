# testingRNG : testing popular random-number generators

There are several benchmarks that can be used to test (pseudo-)random number
generators (RNG). Of particular interest are TestU01 and PractRand. We want to test these popular RNGs.

## Table of contents

- [Scope Limitation](#scope-limitation)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Usage](#usage)
- [The contenders](#the-contenders)
- [Methodology](#methodology)
- [TestU01 results](#testu01-results)
- [PractRand results (512 GB)](#practrand-results-512-gb)
- [Speed results](#speed-results)
- [Visual Summary](#visual-summary)
- [Interpreting the results](#interpreting-the-results)
- [Contributing a new generator](#contributing-a-new-generator)
- [Testing frameworks](#testing-frameworks)
- [Academic references](#academic-references)
- [Talks](#talks)
- [Cite this work](#cite-this-work)
- [Links of interest (technical)](#links-of-interest-technical)
- [Blog posts](#blog-posts)
- [More reading (interesting quotes)](#more-reading-interesting-quotes)
- [Publications](#publications)
- [How to cite?](#how-to-cite)

## Scope Limitation

This project is meant to test some well-known non-cryptographic random number generators written in C/C++ using pre-existing frameworks (TestU01 and PractRand). If you would like to contribute new RNG functions, please open a pull request.

This project is not a tutorial on how to use TestU01 and PractRand. If you have questions about how to use TestU01 and PractRand, or have issues with TestU01 and PractRand, please refer to the relevant projects. We have not modified, in the least, the TestU01 and PractRand frameworks.



## Prerequisites

We assume Linux or macOS. Under Windows 10, you can get the [Linux Subsystem](https://msdn.microsoft.com/en-us/commandline/wsl/install_guide).

- Our scripts should work out of the box on  most Linux boxes, except maybe when they are configured to act as secure servers (e.g., without a C compiler).
- If you have a mac, it is assumed that [you have installed a recent Xcode package](https://developer.apple.com/xcode/) to get
the C (and C++) compiler. Make sure you have installed the command-line utilities.
- C++ compiler supporting C++23 (GCC 14+ or Clang 17+ recommended)
- CMake (version 3.10 or later)
- Perl interpreter (for summarizing TestU01 results)
- Bash shell (for running test scripts)

(Note: We assume a recent x64 processor. TestU01, in particular, does not easily build on some ARM-based systems.)

## Building

We use CMake as our build system. From the project root:

```
cmake -B build
cmake --build build
ctest --test-dir build
```

By default, the build compiles in Release mode and builds everything: speed benchmarks, unit tests, PractRand test harnesses, entropy test harnesses, and TestU01 test harnesses. TestU01 is built automatically from the included `testu01/TestU01.zip` archive.

### CMake options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_SPEED` | ON | Speed benchmarks |
| `BUILD_UNIT` | ON | Unit tests |
| `BUILD_PRACTRAND` | ON | PractRand test harnesses |
| `BUILD_ENTROPY` | ON | Entropy test harnesses |
| `BUILD_TESTU01` | ON | TestU01 test harnesses (built from included archive) |

## Usage

Scripts are copied into the build directory alongside the executables during configuration. After building, you can run them directly from there.

### Speed:
```
cmake -B build
cmake --build build
./build/speed/rng
```

If you can run `rng` in a privileged manner, you will get performance counters. On some systems,
you may need to run `sudo ./build/speed/rng`.

### PractRand:
```
cmake -B build
cmake --build build
bash build/practrand/runtests.sh
```

**NOTE**. You may encounter issues with `PractRand` like memory corruption or the like. If you do, please do report the errors with the authors of the `PractRand` tool.

To summarize the results, use the ``summarize.sh`` script in the ``results`` directory. After running the tests, the log files will be in ``build/practrand/``, and ``summarize.sh`` is located in ``build/practrand/results/`` (copied from ``practrand/results`` during build configuration). To summarize your test results, run ``cd build/practrand/results && ./summarize.sh``.

The PractRand benchmark takes some time to complete because we analyze a large volume of random numbers.

### TestU01:

```
cmake -B build
cmake --build build
bash build/testu01/bigcrushall.sh
```

The TestU01 benchmark "big crush" (``bigcrushall.sh``) might take days. It outputs its results in the current
directory, but we copied already computed results in the ``results`` subdirectory.
A parallel version (``bigcrushallparallel.sh``) will test multiple generators at the same time, up to the number of detected CPU threads.

To summarize the results, use the ``summarize.pl`` script in the ``results`` directory: ``./summarize.pl *.log``. After running the tests, the log files will be in ``build/testu01/``, and ``summarize.pl`` is located in ``build/testu01/results/`` (copied from ``testu01/results`` during build configuration). To summarize your test results, run ``cd build/testu01/results && ./summarize.pl ../*.log``.


### Entropy:
```
cmake -B build
cmake --build build
bash ./build/entropy/runtests.sh
```

## The contenders

- splitmix64 is a random number generator in widespread use and part of the standard Java API, we adapted a port to C produced by Vigna. It produces 64-bit numbers.
- splitmix63 is a 63-bit variant of splitmix64, masking the output to avoid the most significant bit.
- pcg32 and pcg64 are instances of the PCG family designed by O'Neill. They produce either 32-bit or 64-bit outputs.
- xorshift32 is a classical xorshift random number generator. We do not expect it to do well.
- xorshift_k4 and xorshift_k5 are compact xorshift variants producing 32-bit outputs.
- xorshift128plus, xorshift1024star, xorshift1024plus and xoroshiro128plus are recently proposed random number generators by Vigna. There are many parameters possible, but we used those recommended by Vigna. For xorshift128plus, the V8 JavaScript runtime opted for other constants, so we add a new generator "v8xorshift128plus" which relies on constants that Vigna recommended against using, but that are apparently used by V8.
- trivium32 and trivium64 are stream ciphers adapted as random number generators.
- rand is whatever random number generator your C standard library provides. It is a useful point of reference when assessing speed.
- lehmer64 is a simple (but fast) Multiplicative Linear Congruential Generator.
- mersennetwister is the classic Mersenne Twister random number generator.
- mitchellmoore is Mitchell-Moore's generator.
- widynski is Widynski's random number generator.
- aesctr and aesdragontamer are random number generators based on the AES cipher (contributed by Samuel Neves).
- wyhash64 is wyhash hashing function adapted as a random number generator.
- wyrand and w1rand are random number generators based on the MUM hashing function, with w1rand being a slight modification.
- jenkinssmall is Bob Jenkins' small PRNG.
- CG64, CG128, and CG128_64 are counter-based generators producing 64-bit, 128-bit, and 128-bit outputs respectively.


## Methodology

For historical reasons, it appears that TestU01 is designed to test 32-bit numbers
whereas many modern random number generators produce 64-bit numbers. Indeed, years ago,
most random number generators would produce, at best 31-bit random values.

How do we assess modern-day 64-bit random number generators? In such cases,
a sensible approach is to "cast" the result of the 64-bit test to a 32-bit integer.
(In C/C++, we do ``uint32_t x32 = (uint32_t) x;``.)
However, as pointed out by [Vigna (2016)](https://arxiv.org/pdf/1402.6246.pdf), we should
make sure that permuting the bit order does not lead to a test failure. So we test with
both the original, and reversed bit order. We also test with a reversed byte order.

We can also test the most significant bits (msb) instead of the least significant bits (lsb)
(In C/C++, we do ``uint32_t x32 = (uint32_t) (x >> 32)``.) By convention, we refer to this
approach with the ``-H`` flag in our command-line executables.

There are other possibilities, but if a random number generator were to require a very
particular approach to extract good 32-bit values from a 64-bit value, then it would be
a good sign that something is not quite right with the original 64-bit values.

A given tests might fail with one seed, but this tells us little. So we check that
the test fails with at least four seeds.

For PractRand, we do not need to truncate the produced random bits.

## TestU01 results

See testu01/results for detailed outputs.
Type ``./summarize.pl *.log |more``.
There are also supplementary results in the ``longresults`` subdirectory for
specific generators, to confirm beyond a doubt systematic failures.

```
$ ./summarize.pl testsplitmix64-*.log
Summary for splitmix64 lsb 32-bits (4 crushes):
- 3 unnoteworthy blips (#31, #40, #87)

Summary for splitmix64 lsb 32-bits (bit reverse) (4 crushes):
- 1 unnoteworthy blips (#16)

Summary for splitmix64 lsb 32-bits (byte reverse) (4 crushes):
- 1 unnoteworthy blips (#49)

Summary for splitmix64 msb 32-bits (4 crushes):
- 2 unnoteworthy blips (#102, #102)

Summary for splitmix64 msb 32-bits (bit reverse) (4 crushes):
- 3 unnoteworthy blips (#11, #65, #87)

Summary for splitmix64 msb 32-bits (byte reverse) (4 crushes):
- 4 unnoteworthy blips (#11, #38, #58, #74)
```

```
$ ./summarize.pl testpcg32*.log
Summary for pcg32 lsb 32-bits (4 crushes):
- 1 unnoteworthy blips (#9)

Summary for pcg32 lsb 32-bits (bit reverse) (4 crushes):
- 1 unnoteworthy blips (#23)
```

```
$ ./summarize.pl testpcg64*.log
Summary for pcg64 lsb 32-bits (bit reverse) (4 crushes):
- 2 unnoteworthy blips (#49, #74)

Summary for pcg64 lsb 32-bits (byte reverse) (4 crushes):
- 1 unnoteworthy blips (#38)

Summary for pcg64 msb 32-bits (4 crushes):
- 1 unnoteworthy blips (#6)

Summary for pcg64 msb 32-bits (bit reverse) (4 crushes):
- 2 unnoteworthy blips (#33, #76)

Summary for pcg64 msb 32-bits (byte reverse) (4 crushes):
- 1 unnoteworthy blips (#7)
```

```
$ ./summarize.pl testxoroshiro*.log
Summary for xoroshiro128plus lsb 32-bits (4 crushes):
- 1 unnoteworthy blips (#76)

Summary for xoroshiro128plus lsb 32-bits (bit reverse) (4 crushes):
- #68: MatrixRank, L=1000, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1) -- ALL CRUSHES FAIL!!

Summary for xoroshiro128plus lsb 32-bits (byte reverse) (4 crushes):
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!

Summary for xoroshiro128plus msb 32-bits (4 crushes):
- 4 unnoteworthy blips (#11, #38, #97, #98)

Summary for xoroshiro128plus msb 32-bits (bit reverse) (4 crushes):
- 2 unnoteworthy blips (#48, #88)
```

```
$ ./summarize.pl testxorshift128plus*.log
reviewing xorshift128plus lsb 32-bits
Summary for xorshift128plus lsb 32-bits (4 crushes):
- 3 unnoteworthy blips (#7, #22, #59)

reviewing xorshift128plus lsb 32-bits (bit reverse)
Summary for xorshift128plus lsb 32-bits (bit reverse) (4 crushes):
- #68: MatrixRank, L=1000, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1) -- ALL CRUSHES FAIL!!
- 2 unnoteworthy blips (#24, #24)

reviewing xorshift128plus lsb 32-bits (byte reverse)
Summary for xorshift128plus lsb 32-bits (byte reverse) (4 crushes):
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
```
```
$ ./summarize.pl testv8xorshift128plus-*.logreviewing v8xorshift128plus lsb 32-bits
Summary for v8xorshift128plus lsb 32-bits (4 crushes):
- 2 unnoteworthy blips (#47, #50)

reviewing v8xorshift128plus lsb 32-bits (bit reverse)
Summary for v8xorshift128plus lsb 32-bits (bit reverse) (4 crushes):
- #68: MatrixRank, L=1000, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1) -- ALL CRUSHES FAIL!!
- 3 unnoteworthy blips (#24, #43, #78)

reviewing v8xorshift128plus lsb 32-bits (byte reverse)
Summary for v8xorshift128plus lsb 32-bits (byte reverse) (4 crushes):
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- 2 unnoteworthy blips (#9, #102)

reviewing v8xorshift128plus msb 32-bits (bit reverse)
Summary for v8xorshift128plus msb 32-bits (bit reverse) (4 crushes):
- 1 unnoteworthy blips (#11)

reviewing v8xorshift128plus msb 32-bits (byte reverse)
Summary for v8xorshift128plus msb 32-bits (byte reverse) (4 crushes):
- 1 unnoteworthy blips (#48)
```


```
$ ./summarize.pl testlehmer64*.log
reviewing lehmer64 lsb 32-bits
Summary for lehmer64 lsb 32-bits (4 crushes):
- 2 unnoteworthy blips (#50, #74)

reviewing lehmer64 lsb 32-bits (bit reverse)
Summary for lehmer64 lsb 32-bits (bit reverse) (4 crushes):
- 1 unnoteworthy blips (#77)

reviewing lehmer64 lsb 32-bits (byte reverse)
Summary for lehmer64 lsb 32-bits (byte reverse) (4 crushes):
- 3 unnoteworthy blips (#59, #64, #76)

reviewing lehmer64 msb 32-bits (bit reverse)
Summary for lehmer64 msb 32-bits (bit reverse) (4 crushes):
- 4 unnoteworthy blips (#10, #74, #80, #91)

reviewing lehmer64 msb 32-bits (byte reverse)
Summary for lehmer64 msb 32-bits (byte reverse) (4 crushes):
- 1 unnoteworthy blips (#12)
```
```
$ ./summarize.pl testaesctr*.log
reviewing aesctr lsb 32-bits
Summary for aesctr lsb 32-bits (4 crushes):
- 3 unnoteworthy blips (#10, #22, #88)

reviewing aesctr lsb 32-bits (bit reverse)
Summary for aesctr lsb 32-bits (bit reverse) (4 crushes):
- 1 unnoteworthy blips (#11)
```

```
$ ./results/summarize.pl testxorshift1024star*.log
reviewing xorshift1024star lsb 32-bits
Summary for xorshift1024star lsb 32-bits (4 crushes):
- #81: LinearComp, r = 29: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1) -- ALL CRUSHES FAIL!!

reviewing xorshift1024star lsb 32-bits (bit reverse)
Summary for xorshift1024star lsb 32-bits (bit reverse) (4 crushes):
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1) -- ALL CRUSHES FAIL!!
- 2 unnoteworthy blips (#9, #75)

reviewing xorshift1024star msb 32-bits
Summary for xorshift1024star msb 32-bits (4 crushes):
- 2 unnoteworthy blips (#9, #48)

reviewing xorshift1024star msb 32-bits (bit reverse)
Summary for xorshift1024star msb 32-bits (bit reverse) (4 crushes):
- 5 unnoteworthy blips (#7, #23, #78, #99, #104)
```
```
reviewing xorshift1024plus lsb 32-bits (bit reverse)
Summary for xorshift1024plus lsb 32-bits (bit reverse) (4 crushes):
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1) -- ALL CRUSHES FAIL!!
- 1 unnoteworthy blips (#74)

reviewing xorshift1024plus msb 32-bits
Summary for xorshift1024plus msb 32-bits (4 crushes):
- 2 unnoteworthy blips (#77, #85)

reviewing xorshift1024plus msb 32-bits (bit reverse)
Summary for xorshift1024plus msb 32-bits (bit reverse) (4 crushes):
- 5 unnoteworthy blips (#18, #21, #25, #51, #55)

reviewing xorshift1024plus msb 32-bits (byte reverse)
Summary for xorshift1024plus msb 32-bits (byte reverse) (4 crushes):
- 1 unnoteworthy blips (#95)
```

The xorshift32 generator fails very badly.

## PractRand results (512 GB)

See practrand/results for detailed outputs.

Raw output:

```
$ ./summarize.sh 
testmersennetwister.log:  [Low16/64]BRank(12):12K(1)        R= +3016  p~=  6.7e-909   FAIL !!!!!!!
testmitchellmoore.log:  [Low1/64]BRank(12):256(2)         R= +73.5  p~=  3.8e-23    FAIL !!
testv8xorshift128plus-H.log:  BCFN(2+1,13-0,T)                  R= +28.7  p =  6.9e-15    FAIL !
testv8xorshift128plus.log:  [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
testxoroshiro128plus.log:  [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
testxorshift1024plus.log:  [Low1/64]BRank(12):1536(1)        R=+10916  p~=  3e-3287    FAIL !!!!!!!!
testxorshift1024star.log:  [Low4/64]BRank(12):1536(1)        R= +2650  p~=  9.8e-799   FAIL !!!!!!!
testxorshift128plus-H.log:  BCFN(2+1,13-0,T)                  R= +27.9  p =  1.9e-14    FAIL
testxorshift128plus.log:  [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
testxorshift32.log:  BCFN(2+0,13-2,T)                  R=+179.4  p =  2.8e-91    FAIL !!!!!
testxorshift-k4.log:  BRank(12):256(4)                  R= +5300  p~=  1e-2819    FAIL !!!!!!!!
testxorshift-k5.log:  [Low4/64]BRank(12):768(1)         R=+583.3  p~=  1.2e-176   FAIL !!!!!!
```



## Speed results

For a report on what might be the fastest generator, see [The fastest conventional random number generator that can pass Big Crush?](https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/)

On an Intel Xeon Gold 6548N (2.8 GHz) with GCC 14, we get the following results:

```
32-bit generators:
trivium                                  :   1.09 ns/byte   0.92 GB/s   3.42 GHz   3.82 c/b  20.26 i/b   5.31 i/c
xorshift_k4                              :   0.64 ns/byte   1.55 GB/s   3.45 GHz   2.25 c/b   5.76 i/b   2.56 i/c
xorshift_k5                              :   0.64 ns/byte   1.55 GB/s   3.47 GHz   2.25 c/b   6.76 i/b   3.00 i/c
mersennetwister                          :   0.60 ns/byte   1.68 GB/s   3.46 GHz   2.09 c/b   7.75 i/b   3.71 i/c
mitchellmoore                            :   0.61 ns/byte   1.64 GB/s   3.47 GHz   2.13 c/b  11.76 i/b   5.51 i/c
widynski                                 :   0.72 ns/byte   1.40 GB/s   3.49 GHz   2.50 c/b   2.76 i/b   1.10 i/c
xorshift32                               :   0.79 ns/byte   1.27 GB/s   3.49 GHz   2.76 c/b   4.26 i/b   1.54 i/c
pcg32                                    :   0.65 ns/byte   1.55 GB/s   3.49 GHz   2.25 c/b   4.26 i/b   1.89 i/c
rand                                     :   3.59 ns/byte   0.28 GB/s   3.49 GHz  12.55 c/b  15.25 i/b   1.22 i/c

64-bit generators:
trivium64                                :   0.63 ns/byte   1.58 GB/s   3.49 GHz   2.21 c/b   9.01 i/b   4.08 i/c
aesdragontamer                           :   0.25 ns/byte   3.97 GB/s   3.49 GHz   0.88 c/b   1.82 i/b   2.06 i/c
aesctr                                   :   0.18 ns/byte   5.50 GB/s   3.49 GHz   0.64 c/b   2.90 i/b   4.55 i/c
lehmer64                                 :   0.32 ns/byte   3.10 GB/s   3.49 GHz   1.13 c/b   1.76 i/b   1.56 i/c
xorshift128plus                          :   0.42 ns/byte   2.41 GB/s   3.49 GHz   1.45 c/b   2.63 i/b   1.81 i/c
xoroshiro128plus                         :   0.39 ns/byte   2.54 GB/s   3.48 GHz   1.38 c/b   2.51 i/b   1.82 i/c
splitmix64                               :   0.21 ns/byte   4.70 GB/s   3.49 GHz   0.74 c/b   2.76 i/b   3.70 i/c
splitmix63                               :   0.22 ns/byte   4.50 GB/s   3.49 GHz   0.78 c/b   3.26 i/b   4.19 i/c
pcg64                                    :   0.40 ns/byte   2.53 GB/s   3.49 GHz   1.38 c/b   3.26 i/b   2.36 i/c
xorshift1024star                         :   0.49 ns/byte   2.03 GB/s   3.49 GHz   1.72 c/b   3.88 i/b   2.25 i/c
xorshift1024plus                         :   0.32 ns/byte   3.10 GB/s   3.49 GHz   1.13 c/b   3.01 i/b   2.67 i/c
wyhash64                                 :   0.23 ns/byte   4.28 GB/s   3.49 GHz   0.82 c/b   1.88 i/b   2.30 i/c
wyrand                                   :   0.24 ns/byte   4.22 GB/s   3.49 GHz   0.83 c/b   1.76 i/b   2.12 i/c
w1rand                                   :   0.24 ns/byte   4.19 GB/s   3.49 GHz   0.83 c/b   1.76 i/b   2.11 i/c
jenkinssmall                             :   0.74 ns/byte   1.36 GB/s   3.49 GHz   2.58 c/b   3.26 i/b   1.26 i/c
CG64                                     :   0.47 ns/byte   2.11 GB/s   3.49 GHz   1.66 c/b   2.76 i/b   1.66 i/c

128-bit generators:
CG128                                    :   0.27 ns/byte   3.70 GB/s   3.49 GHz   0.94 c/b   3.07 i/b   3.25 i/c
CG128_64                                 :   0.22 ns/byte   4.56 GB/s   3.49 GHz   0.77 c/b   2.00 i/b   2.62 i/c
```

Results will depend on your specific hardware and might be quite different on ARM processors. Tweaking the benchmark could also change the results. In particular, our benchmark stresses throughput as opposed to latency.


## Visual Summary

|                   | TestU01 (big crush)| PractRand (512 GB)       | cycles/byte |  GB/s |
|-------------------|--------------------|--------------------------| -----------:|------:|
| aesctr            |  :+1:              |   :+1:                   | 0.64        |  5.50 |
| splitmix64        |  :+1:              |   :+1:                   | 0.74        |  4.70 |
| wyhash64          |  :+1:              |   :+1:                   | 0.82        |  4.28 |
| wyrand            |  :+1:              |   :+1:                   | 0.83        |  4.22 |
| w1rand            |  :+1:              |   :+1:                   | 0.83        |  4.19 |
| aesdragontamer    |  :+1:              |   :+1:                   | 0.88        |  3.97 |
| lehmer64          |  :+1:              |   :+1:                   | 1.13        |  3.10 |
| xorshift1024plus  |  fails!            |   fails!                 | 1.13        |  3.10 |
| pcg64             |  :+1:              |   :+1:                   | 1.38        |  2.53 |
| xoroshiro128plus  |  fails!            |   fails!                 | 1.38        |  2.54 |
| xorshift128plus   |  fails!            |   fails!                 | 1.45        |  2.41 |
| xorshift1024star  |  fails!            |   fails!                 | 1.72        |  2.03 |
| pcg32             |  :+1:              |   :+1:                   | 2.25        |  1.55 |
| xorshift32        |  fails!            |   fails!                 | 2.76        |  1.27 |

## Interpreting the results

Tests are subject to both false positives and false negatives, and should be interpreted with care.

- That your random number generator passes several statistical tests does not prove that it is of high quality. There might be other tests that it would fail. However, the more tests one passes, the greater the evidence is in favor of your random number generator, evidently.
- A statistical test might fail even if the bits are indeed "random". Thus a failed test is not "proof" that the random number generation is faulty. However, failing tests can be further investigated and evidence of a fault can be ascertained.

One source of concern is that random number generators are initialized with a seed, and sometimes a "sequence" is selected. Testing successfully with a given seed does not preclude the possibility that there might be "bad seeds". However, if we pick different seeds an a failure keeps happening, we gain confidence that the failure is easily reproducible and thus problematic. Repeated test failures with different seeds gives us confidence that there is a fault.

To summarize, caution is required when interpreting the results. It is not black and white, good and bad... One might say that a given generator passes a given test, but it is possible that with different seeds, it could fail, and so forth.

Still, for convenience, it is necessary to express results in a comprehensible manner. Thus we often say that a generator "passes a given test" or does not.



## Contributing a new generator

To add a new random number generator, follow these steps:

### 1. Create a header file in `source/`

Create `source/mynewthing.h` following the existing pattern. Your header should provide:

- A seed function (e.g., `mynewthing_seed(uint64_t seed)`)
- A generation function that returns `uint32_t` or `uint64_t` (e.g., `mynewthing()`)
- Use `static inline` functions and `static` global state
- Use `splitmix64` for seeding (include `splitmix64.h`)

Example skeleton for a 64-bit generator:
```c
#ifndef MYNEWTHING_H
#define MYNEWTHING_H

#include "splitmix64.h"

static uint64_t g_mynewthing_state;

static inline void mynewthing_seed(uint64_t seed) {
  g_mynewthing_state = splitmix64_stateless(seed, 0);
}

static inline uint64_t mynewthing() {
  // your generator logic here
}

#endif
```

### 2. Add to the speed benchmark

Edit `speed/src/rng.cpp`: include your header and add an entry to the appropriate generators array (`generators32`, `generators64`, or `generators128`).

### 3. Add a PractRand test harness

Create `practrand/src/testmynewthing.c`:
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mynewthing.h"

#define buffer_size 512
int main() {
  mynewthing_seed(12345678);
  uint64_t buffer[buffer_size];
  while (1) {
    for (int k = 0; k < buffer_size; k++)
      buffer[k] = mynewthing();
    fwrite((void *)buffer, sizeof(buffer), 1, stdout);
  }
}
```

Then add `testmynewthing` to the list in `practrand/CMakeLists.txt`.

### 4. Add a TestU01 test harness

Create `testu01/src/testmynewthing.c`:
```c
#include "mynewthing.h"

static inline void thisrng_seed(uint64_t seed) { mynewthing_seed(seed); }
static inline uint64_t thisrng() { return mynewthing(); }
const char *name = "mynewthing";

#include "main.h"
```

Then add `testmynewthing` to the list in `testu01/CMakeLists.txt`.

### 5. Build and verify

```
cmake -B build
cmake --build build
ctest --test-dir build
./build/speed/rng
```

### 6. Open a pull request

Please include the name of the generator, a reference to the original publication or source, and your benchmark results.

## Testing frameworks

We use the following testing framework:

- [Practically Random](https://sourceforge.net/projects/pracrand/)
- [TestU01](http://simul.iro.umontreal.ca/testu01/)

As of 2026, these represent the state-of-the-art.

## Academic references

- D. H. Lehmer, Mathematical methods in large-scale computing units.  Proceedings of a Second Symposium on Large Scale Digital Calculating Machinery; Annals of the Computation Laboratory, Harvard Univ. 26 (1951), pp. 141-146.


## Talks

- [Stanford Seminar - PCG: A Family of Better Random Number Generators](https://www.youtube.com/watch?v=45Oet5qjlms) (by O'Neill)

## Cite this work

If you use results or code from this repository in a publication, please consider citing the project as:

```
@misc{lemire2025testingRNG,
  author = {Daniel Lemire},
  title = {{testingRNG: testing popular random-number generators}},
  year = {2025},
  howpublished = {GitHub repository},
  note = {Commit on 2025-12-22},
  url = {https://github.com/dlemire/testingRNG}
}
```

You can copy the BibTeX entry above into your references file.

## Links of interest (technical)

- [PRNG shootout](http://xoroshiro.di.unimi.it) by Vigna
- How to test PCG with  [PractRand](http://www.pcg-random.org/posts/how-to-test-with-practrand.html) and [TestU01](http://www.pcg-random.org/posts/how-to-test-with-testu01.html) (by O'Neill)

## Blog posts

- [Testing non-cryptographic random number generators: my results](https://lemire.me/blog/2017/08/22/testing-non-cryptographic-random-number-generators-my-results/) by Lemire
- [Cracking random number generators (xoroshiro128+)](https://lemire.me/blog/2017/08/22/cracking-random-number-generators-xoroshiro128/) by Lemire
- [On Melissa O’Neill’s PCG random number generator](http://lemire.me/blog/2017/08/15/on-melissa-oneills-pcg-random-number-generator/) by Lemire
- [Testing RNGs with PractRand](https://www.johndcook.com/blog/2017/08/14/testing-rngs-with-practrand/), [Manipulating a random number generator](https://www.johndcook.com/blog/2017/08/16/manipulating-a-random-number-generator/) by Cook
- [PCG Passes PractRand](http://www.pcg-random.org/posts/pcg-passes-practrand.html) by O'Neill

## More reading (interesting quotes)

> When a p-value is extremely close to 0 or to 1 (for example, if it is less than 10^−10), one can obviously conclude that the generator fails the test. If the p-value is suspicious but failure is not clear enough, (p = 0.0005, for example), then the test can be replicated independently until either failure becomes obvious or suspicion disappears (i.e., one finds that the suspect p-value was obtained only by chance). This approach is possible because there is no limit (other than CPU time) on the amount of data that can be produced by a RNG to increase the sample size and the power of the test. (TestU01 manual)

> A recent example shows the importance of testing the reverse generator. Saito and Matsumoto [2014] propose a different way to eliminate linear artifacts: instead of multiplying the output of an underlying xorshift generator (with 128 bits of state and based on 32-bit shifts) by a constant, they add it (in Z/232Z) with the previous output. Since the sum in Z/232Z is not linear over Z/2Z, the result should be free of linear artifacts. However, while their generator passes BigCrush, its reverse fails systematically the LinearComp, MatrixRank, MaxOft and Permutation test of BigCrush, which highlights a significant weakness in its lower bits. [Vigna, An experimental exploration of Marsaglia’s xorshift generators, scrambled, ACM Transactions on Mathematical Software (TOMS), 2016 ](https://arxiv.org/pdf/1402.6246.pdf)

> xoroshiro128plus seems like an interesting variation on LFSRs, it maintains the excessive linearity problem, but the variation in structure (compared to classic LFSRs) costs little and may significantly improve quality, not sure yet. In PractRand it quickly fails binary rank tests, and also eventually fails a short-medium range linear tests in the expanded test suite (DC6-5x4Byte-1). The author suggests avoiding reliance on the lowest bit to work around the binary rank problem, but the 2nd lowest bit also fails binary rank tests (though it takes substantially larger matrices to detect that). A non-linear output function could fix that stuff, but I'd also prefer a larger statespace. It's not clear how well this PRNG supports random access - a function to skip forward exactly 2^64 is provided, but there's no obvious way to skip forward/backward other amounts - I'd guess it's possible, but requires significant precomputation on a per-power-of-2 basis and might be slow at runtime for distances that are not neat powers of two. I think xoroshiro has a single cycle of length 2^128-1.
[Chris Doty-Humphrey, 2016-09-07](https://sourceforge.net/p/pracrand/discussion/366935/thread/2b43b548/#a33d)


## Publications


- [Xorshift1024*, Xorshift1024+, Xorshift128+ and Xoroshiro128+ fail statistical tests for linearity](https://www.sciencedirect.com/science/article/pii/S0377042718306265?dgcid=author), Journal of Computational and Applied Mathematics, Volume 350, 2019. (Available online 22 October 2018)

## How to cite?

If you use results or code from this repository in a publication, please consider citing the project as:

```
@misc{lemire2026testingRNG,
  author = {Daniel Lemire},
  title = {{testingRNG: testing popular random-number generators}},
  year = {2026},
  howpublished = {GitHub repository},
  url = {https://github.com/dlemire/testingRNG}
}
```

You can copy the BibTeX entry above into your references file.

