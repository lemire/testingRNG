# testingRNG : testing popular random-number generators

There are several benchmarks that can be used to test (pseudo-)random number
generators (RNG). Of particular interest are TestU01 and PractRand. We want to easily
test popular RNGs.

It is fine to say that we can, in theory run these tests, but few of us will if
it requires too much labor. The purpose of this project is to make it ridiculously
easy to run your own tests if you have a mac or a Linux box with a recent C compiler.


This project lead to the following publication:

- [Xorshift1024*, Xorshift1024+, Xorshift128+ and Xoroshiro128+ fail statistical tests for linearity](https://www.sciencedirect.com/science/article/pii/S0377042718306265?dgcid=author), Journal of Computational and Applied Mathematics, to appear (Available online 22 October 2018)

## Prerequisites

We assume Linux or macOS. Under Windows 10, you can get the [Linux Subsystem](https://msdn.microsoft.com/en-us/commandline/wsl/install_guide).

- Our scripts should work out of the box on  most Linux boxes, except maybe when they are configured to act as secure servers (e.g., without a C compiler).
- If you have a mac, it is assumed that [you have installed a recent Xcode package](https://developer.apple.com/xcode/) to get
the C (and C++) compiler. Make sure you have installed the command-line utilities.

(Note: We assume a recent x64 processor. TestU01, in particular, does not easily build on some ARM-based systems.)

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
./bigcrushall.sh
```

The TestU01 benchmark "big crush" (``bigcrushall.sh``) might take days. It outputs its results in the current
directory (``testu1``, but we copied already computed in the ``results`` subdirectory.
A parallel version (``bigcrushallparallel.sh``) will test multiple generators at the same time, up to the number of detected CPU threads.

There are also extensive scripts that generate many (100) seeds and test generators, the script take
the form ``rand*.sh``, one per generator. They output their results in their ``longresults`` subdirectory.

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
- xorshift128plus, xorshift1024star, xorshift1024plus and xoroshiro128plus are recently proposed random number generator by Vigna. There are many parameters possible, but we used those recommended by Vigna. For xorshift128plus, the V8 JavaScript runtime opted for other constants, so we add a new generator "v8xorshift128plus" which relies on constants that Vigna recommended against using, but that are apparently used by V8.
- rand is whatever random number number generator your C standard library provides. It is a useful point of reference when assessing speed.
- lehmer64 is a simple (but fast) Multiplicative Linear Congruential Generator
- aesctr is a random number generator based on the AES cipher (contributed by Samuel Neves)


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

On a recent (Skylake) processor, on a Linux box, I got the following results:


```
xorshift_k4:  1.26 cycles per byte
xorshift_k5:  1.26 cycles per byte
mersennetwister:  2.34 cycles per byte
mitchellmoore:  3.49 cycles per byte
widynski:  1.26 cycles per byte
xorshift32:  2.51 cycles per byte
pcg32:  1.51 cycles per byte
rand:  4.74 cycles per byte
aesdragontamer:  0.92 cycles per byte
aesctr:  1.18 cycles per byte
lehmer64:  0.63 cycles per byte
xorshift128plus:  0.89 cycles per byte
xoroshiro128plus:  0.83 cycles per byte
splitmix64:  1.01 cycles per byte
pcg64:  0.97 cycles per byte
xorshift1024star:  1.74 cycles per byte
xorshift1024plus:  1.03 cycles per byte
wyhash64:  0.76 cycles per byte
```

Results will depend on your specific hardware and might be quite different on ARM processors. Tweaking the benchmark could also change the results. In particular, our benchmark stresses throughput as opposed to latency.


## Visual Summary

|                  | TestU01 (big crush)| PractRand (64 GB)        | time (cycles/byte) |
|------------------|--------------------|--------------------------|--------------------|
| lehmer64         |  :+1:              |   :+1:                   | 1.0                |
| splitmix64       |  :+1:              |   :+1:                   | 1.0                |
| aes              |  :+1:              |   :+1:                   | 1.0                |
| xorshift128plus  |  fails!            |   fails!                 | 1.0                |
| v8xorshift128plus|  fails!            |   fails!                 | 1.0                |
| xorshift1024plus |  fails!            |   fails!                 | 1.0                |
| xoroshiro128plus |  fails!            |   fails!                 | 1.1                |
| pcg64            |  :+1:              |   :+1:                   | 1.4                |
| xorshift1024star |  fails!            |   fails!                 | 1.5                |
| pcg32            |  :+1:              |   :+1:                   | 2.1                |
| xorshift32       |  fails!            |   fails!                 | 2.5                |

## Interpreting the results

Tests are subject to both false positives and false negatives, and should be interpreted with care.

- That your random number generator passes several statistical tests does not prove that it is of high quality. There might be other tests that it would fail. However, the more tests one passes, the greater the evidence is in favor of your random number generator, evidently.
- A statistical test might fail even if the bits are indeed "random". Thus a failed test is not "proof" that the random number generation is faulty. However, failing tests can be further investigated and evidence of a fault can be ascertained.

One source of concern is that random number generators are initialized with a seed, and sometimes a "sequence" is selected. Testing successfully with a given seed does not preclude the possibility that there might be "bad seeds". However, if we pick different seeds an a failure keeps happening, we gain confidence that the failure is easily reproducible and thus problematic. Repeated test failures with different seeds gives us confidence that there is a fault.

To summarize, caution is required when interpreting the results. It is not black and white, good and bad... One might say that a given generator passes a given test, but it is possible that with different seeds, it could fail, and so forth.

Still, for convenience, it is necessary to express results in a comprehensible manner. Thus we often say that a generator "passes a given test" or does not.



## Testing frameworks

We use the following testing framework:

- [Practically Random](https://sourceforge.net/projects/pracrand/)
- [TestU01](http://simul.iro.umontreal.ca/testu01/)

As of 2017, these represent the state-of-the-art.

## Academic references

- D. H. Lehmer, Mathematical methods in large-scale computing units.  Proceedings of a Second Symposium on Large Scale Digital Calculating Machinery; Annals of the Computation Laboratory, Harvard Univ. 26 (1951), pp. 141-146.


## Talks

- [Stanford Seminar - PCG: A Family of Better Random Number Generators](https://www.youtube.com/watch?v=45Oet5qjlms) (by O'Neill)

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
