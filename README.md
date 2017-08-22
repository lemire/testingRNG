# testingRNG

There are several benchmarks that can be used to test (pseudo-)random number
generators (RNG). Of particular interest are TestU01 and PractRand. We want to easily
test popular RNGs.

It is fine to say that we can, in theory run these tests, but few of us will if
it requires too much labor. The purpose of this project is to make it ridiculously
easy to run your own tests if you have a mac or a Linux box with a recent C compiler.

## Prerequisites

We assume Linux or macOS. Under Windows 10, you can get the [Linux Subsystem](https://msdn.microsoft.com/en-us/commandline/wsl/install_guide).

- Our scripts should work out of the box on  most Linux boxes, except maybe when they are configured to act as secure servers (e.g., without a C compiler).
- If you have a mac, it is assumed that [you have installed a recent Xcode package](https://developer.apple.com/xcode/) to get
the C (and C++) compiler. Make sure you have installed the command-line utilities.

## Usage

You can run the tests by going to a bash shell (Terminal) and executing a few commands.

#### PractRand:
```
cd practrand
./runtests.sh
```

The PractRand benchmark takes some time to complete because we analyze a large volume of random numbers.

#### TestU01:
```
cd testu01
./bigcrush.sh
./bigcrush_seed2.sh
./bigcrush_seed3.sh
```

The TestU01 benchmark "big crush" (``bigcrush.sh``) might take days.

#### Speed:
It is interesting to assess running speed as well. This can be done quickly:

```
cd speed
make
make test
```

Note  that the speed tests assume a recent x64 processor (e.g., they would not work
  on ARM processors).

## The contenders

- splitmix64 is a random number generator in widespread use and part of the standard Java API, we adapted a port to C produced by Vigna. It produces 64-bit numbers.
- pcg32 and pcg64 are instances of the PCG family designed by O'Neill. They produce either 32-bit or 64-bit outputs.
- xorshift32 is a classical xorshift random number generator. We do not expect it to do well.
- xorshift128plus and xoroshiro128plus are recently proposed random number generator by Vigna.
- rand is whatever random number number generator your C standard library provides. It is a useful point of reference when assessing speed.


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
the test fails with at least two seeds.

For PractRand, we do not need to truncate the produced random bits.

## TestU01 results


- pcg32 fails CollisionOver
- pcg64 fails Run
- splitmix fails CollisionOver  and MaxOft
- xoroshiro128+ fails CollisionOver, HammingIndep, MatrixRank and RandomWalk1
- xorshift128+ fails CollisionOver and MatrixRank
- xorshift32 fails at almost everything

See testu01/results for detailed outputs.
Type ``python process.py *.log |more``.

## PractRand results

- :exclamation: testxorshift128plus fails.
- :exclamation: testxoroshiro128plus fails (*).
- :+1: pcg32 passes the tests.
- :+1: pcg64 passes the tests.
- :+1: splitmix64 passes the tests.
- :exclamation: testxorshift32 fails (as expected).

*- You can salvage xoroshiro128plus, if you just select the most significant bits (``-H``).

See practrand/results for detailed outputs.

Raw output:

```
$ ./runtests.sh
Testing 64GB  of data per run
Note: running the tests longer could expose new failures.
# RUNNING testxorshift128plus -H Outputting result to  testxorshift128plus-H.log
Failure!
# RUNNING testxorshift128plus Outputting result to  testxorshift128plus.log
Failure!
# RUNNING testxoroshiro128plus -H Outputting result to  testxoroshiro128plus-H.log
Success!
# RUNNING testxoroshiro128plus Outputting result to  testxoroshiro128plus.log
Failure!
# RUNNING testpcg32 Outputting result to  testpcg32.log
Success!
# RUNNING testpcg64 -H Outputting result to  testpcg64-H.log
Success!
# RUNNING testpcg64 Outputting result to  testpcg64.log
Success!
# RUNNING testsplitmix64 -H Outputting result to  testsplitmix64-H.log
Success!
# RUNNING testsplitmix64 Outputting result to  testsplitmix64.log
Success!
# RUNNING testxorshift32 Outputting result to  testxorshift32.log
Failure!
```



## Speed results

On a recent (Skylake) processor, on a Linux box, I got the following results:

```
xorshift32:  2.33 cycles per byte
pcg32:  1.81 cycles per byte
rand:  5.19 cycles per byte
xorshift128plus:  0.91 cycles per byte
xoroshiro128plus:  0.85 cycles per byte
splitmix64:  0.89 cycles per byte
pcg64:  1.27 cycles per byte
```

Sorting the results from the fastest to the slowest...

```
xoroshiro128plus:  0.85 cycles per byte
splitmix64:  0.89 cycles per byte
xorshift128plus:  0.91 cycles per byte
pcg64:  1.27 cycles per byte
pcg32:  1.81 cycles per byte
xorshift32:  2.33 cycles per byte
rand:  5.19 cycles per byte
```

Results will depend on your specific hardware and might be quite different on ARM processors. Tweaking the benchmark could also change the results. In particular, our benchmark stresses throughput as opposed to latency.


## Visual Summary

|                  | TestU01        | PractRand      | time (cycles/byte) |
|------------------|----------------|----------------|--------------------|
| xoroshiro128plus |  fails 4 tests | fails!  | 0.9                |
| splitmix64       |   fails 2 tests   |    :+1:        | 0.9                |
| xorshift128plus  | fails 2 tests  | fails!  | 0.9                |
| pcg64            | fails 1 test  |    :+1:        | 1.3                |
| pcg32            |    fails 1 test   |    :+1:        | 1.8                |
| xorshift32       | :exclamation:  | fails!  | 2.3                |



## Interpreting the results

Tests are subject to both false positives and false negatives, and should be interpreted with care.

- That your random number generator passes several statistical tests does not prove that it is of high quality. There might be other tests that it would fail. However, the more tests one passes, the greater the evidence is in favor of your random number generator, evidently.
- A statistical test might fail even if the bits are indeed "random". Thus a fialed test is not "proof" that the random number generation is faulty. However, failing tests can be further investigated and evidence of a fault can be ascertained.

One source of concern is that random number generators are initialized with a seed, and sometimes a "sequence" is selected. Testing successfully with a given seed does not preclude the possibility that there might be "bad seeds".

To summarize, caution is required when interpreting the results. It is not black and white, good and bad... One might say that a given generator passes a given test, but it is possible that with different seeds, it could fail, and so forth.

Still, for convenience, it is necessary to express results in a comprehensible manner. Thus we often say that a generator "passes a given test" or does not.

Repeated test failures with different seeds gives us confidence that there is a fault.

## Testing frameworks

We use the following testing framework:

- [Practically Random](https://sourceforge.net/projects/pracrand/)
- [TestU01](http://simul.iro.umontreal.ca/testu01/)

As of 2017, these represent the state-of-the-art.

## Talks

- [Stanford Seminar - PCG: A Family of Better Random Number Generators](https://www.youtube.com/watch?v=45Oet5qjlms) (by O'Neill)

## Links of interest (technical)

- [PRNG shootout](http://xoroshiro.di.unimi.it) by Vigna
- How to test PCG with  [PractRand](http://www.pcg-random.org/posts/how-to-test-with-practrand.html) and [TestU01](http://www.pcg-random.org/posts/how-to-test-with-testu01.html) (by O'Neill)

## Blog posts

- [On Melissa O’Neill’s PCG random number generator](http://lemire.me/blog/2017/08/15/on-melissa-oneills-pcg-random-number-generator/)
- [Testing RNGs with PractRand](https://www.johndcook.com/blog/2017/08/14/testing-rngs-with-practrand/), [Manipulating a random number generator](https://www.johndcook.com/blog/2017/08/16/manipulating-a-random-number-generator/) by Cook
- [PCG Passes PractRand](http://www.pcg-random.org/posts/pcg-passes-practrand.html)

## More reading (interesting quotes)

> A recent example shows the importance of testing the reverse generator. Saito and Matsumoto [2014] propose a different way to eliminate linear artifacts: instead of multiplying the output of an underlying xorshift generator (with 128 bits of state and based on 32-bit shifts) by a constant, they add it (in Z/232Z) with the previous output. Since the sum in Z/232Z is not linear over Z/2Z, the result should be free of linear artifacts. However, while their generator passes BigCrush, its reverse fails systematically the LinearComp, MatrixRank, MaxOft and Permutation test of BigCrush, which highlights a significant weakness in its lower bits. [Vigna, An experimental exploration of Marsaglia’s xorshift generators, scrambled, ACM Transactions on Mathematical Software (TOMS), 2016 ](https://arxiv.org/pdf/1402.6246.pdf)

> xoroshiro128plus seems like an interesting variation on LFSRs, it maintains the excessive linearity problem, but the variation in structure (compared to classic LFSRs) costs little and may significantly improve quality, not sure yet. In PractRand it quickly fails binary rank tests, and also eventually fails a short-medium range linear tests in the expanded test suite (DC6-5x4Byte-1). The author suggests avoiding reliance on the lowest bit to work around the binary rank problem, but the 2nd lowest bit also fails binary rank tests (though it takes substantially larger matrices to detect that). A non-linear output function could fix that stuff, but I'd also prefer a larger statespace. It's not clear how well this PRNG supports random access - a function to skip forward exactly 2^64 is provided, but there's no obvious way to skip forward/backward other amounts - I'd guess it's possible, but requires significant precomputation on a per-power-of-2 basis and might be slow at runtime for distances that are not neat powers of two. I think xoroshiro has a single cycle of length 2^128-1.
[Chris Doty-Humphrey, 2016-09-07](https://sourceforge.net/p/pracrand/discussion/366935/thread/2b43b548/#a33d)
