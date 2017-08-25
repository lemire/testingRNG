# testingRNG : testing popular random-number generators

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

- lehmer64 is a simple (but fast) Multiplicative Linear Congruential Generator
- aesctr is a random number generator based on the AES cipher (contributed by Samuel Neves)
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

See testu01/results for detailed outputs.
Type ``./summarize.pl *.log |more``.


```

- 2 unnoteworthy blips (#11, #47)

Summary for pcg64 lsb 32-bits (byte reverse) (6 crushes):
- #38: Run, r = 0: investigate? -- too many unusual p-values (0.9998, 0.9998, 0.9998)
- 1 unnoteworthy blips (#22)

Summary for splitmix64 lsb 32-bits (10 crushes):
- #1: SerialOver, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #2: SerialOver, r = 22: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #3: CollisionOver, t = 2: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #4: CollisionOver, t = 2: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #5: CollisionOver, t = 3: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #6: CollisionOver, t = 3: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #7: CollisionOver, t = 7: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #8: CollisionOver, t = 7: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #9: CollisionOver, t = 14: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #10: CollisionOver, t = 14: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #11: CollisionOver, t = 21: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #12: CollisionOver, t = 21: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #13: BirthdaySpacings, t = 2: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #14: BirthdaySpacings, t = 3: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #15: BirthdaySpacings, t = 4: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #16: BirthdaySpacings, t = 7: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #17: BirthdaySpacings, t = 7: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #18: BirthdaySpacings, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #19: BirthdaySpacings, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #20: BirthdaySpacings, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #21: BirthdaySpacings, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #22: ClosePairs mNP2S, t = 3: FAIL!! -- p-values too unlikely (2.8e-192, 1.4e-300, 1.8e-300, 4.2e-16, 1 - eps1, 3.2e-73, 3.1e-241, eps, eps, 2.6e-18, 1 - eps1, 4.3e-91, 1.2e-297, 8.9e-273, 1.1e-278, 5.3e-26, 1 - eps1, 1.9e-94, 9.2e-231, 1.4e-263, 2.3e-277, 1.4e-22, 1 - eps1, 4.9e-105, 9.2e-231, 1.4e-263, 2.3e-277, 1.4e-22, 1 - eps1, 4.9e-105)
- #23: ClosePairs mNP2S, t = 5: FAIL!! -- p-values too unlikely (2.3e-93, eps, eps, 1 - eps1, 9.4e-88, eps, eps, 1 - eps1, 1.3e-89, eps, eps, 1 - eps1, 3.4e-112, eps, eps, 1 - eps1, 3.4e-112, eps, eps, 1 - eps1)
- #24: ClosePairs mNP2S, t = 9: FAIL!! -- p-values too unlikely (5.3e-39, 2.7e-39, 1.2e-79, 1.2e-194, 1.4e-39, 4.4e-43, 1.9e-76, 8.5e-208, 1.4e-31, 2.8e-36, 2.4e-74, 5.2e-229, 7.6e-41, 3.7e-44, 1.4e-95, 5.0e-251, 7.6e-41, 3.7e-44, 1.4e-95, 5.0e-251)
- #25: ClosePairs NJumps, t = 16: investigate! -- too many unusual p-values (9.0e-5, 1.4e-5, 1 -  6.5e-5, 8.2e-4)
- #34: Gap, r = 0: FAIL!! -- p-values too unlikely (4.5e-12, 3.7e-12)
- #35: Gap, r = 25: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #36: Gap, r = 0: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #37: Gap, r = 20: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #42: Permutation, t = 7: FAIL!! -- p-values too unlikely (1 - 3.9e-13, 1 - 3.9e-13)
- #43: Permutation, t = 10: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #46: MaxOft, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #47: MaxOft, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #48: MaxOft, t = 24: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #49: MaxOft, t = 32: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #65: SumCollector: investigate!! -- too many unusual p-values (2.4e-4, 5.8e-4, 3.7e-4, 6.8e-4, 6.8e-4)
- #66: MatrixRank, L=30, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #67: MatrixRank, L=30, r=26: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #68: MatrixRank, L=1000, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #69: MatrixRank, L=1000, r=26: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #70: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps)
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #81: LinearComp, r = 29: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1, 1 - eps1)
- #84: Fourier3, r = 0: investigate? -- too many unusual p-values (1.7e-6, 1.1e-5, 7.7e-4)
- #101: Run of bits, r = 0: investigate!! -- too many unusual p-values (1 -  3.8e-6, 1 -  4.1e-6, 1 -  6.1e-9, 1 -  3.0e-5, 1 -  3.0e-5)
- #102: Run of bits, r = 27: investigate!! -- too many unusual p-values (0.9997, 0.9998, 0.9995, 1 -  4.9e-6, 1 -  4.9e-6)
- 4 unnoteworthy blips (#39, #39, #45, #76)

Summary for splitmix64 lsb 32-bits (bit reverse) (5 crushes):
- #1: SerialOver, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #2: SerialOver, r = 22: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #3: CollisionOver, t = 2: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #4: CollisionOver, t = 2: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #5: CollisionOver, t = 3: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #6: CollisionOver, t = 3: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #7: CollisionOver, t = 7: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #8: CollisionOver, t = 7: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #9: CollisionOver, t = 14: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #10: CollisionOver, t = 14: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #11: CollisionOver, t = 21: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #12: CollisionOver, t = 21: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #13: BirthdaySpacings, t = 2: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #14: BirthdaySpacings, t = 3: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #15: BirthdaySpacings, t = 4: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #16: BirthdaySpacings, t = 7: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #17: BirthdaySpacings, t = 7: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #18: BirthdaySpacings, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #19: BirthdaySpacings, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #20: BirthdaySpacings, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #21: BirthdaySpacings, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #22: ClosePairs mNP2S, t = 3: FAIL!! -- p-values too unlikely (1.3e-40, eps, eps, eps, eps, eps, 3.7e-41, eps, eps, eps, eps, eps, 4.4e-41, eps, eps, eps, eps, eps)
- #23: ClosePairs mNP2S, t = 5: FAIL!! -- p-values too unlikely (1.3e-235, 1.8e-253, 1.2e-267, 1 - eps1, 3.2e-174, 7.0e-231, 5.8e-245, 1 - eps1, 6.0e-223, 2.5e-232, 2.1e-230, 1 - eps1)
- #24: ClosePairs mNP2S, t = 9: FAIL!! -- p-values too unlikely (4.5e-26, 3.6e-18, 9.9e-20, 1 - eps1, 1.5e-17, 2.1e-24, 4.3e-26, 1 - eps1, 2.2e-28, 1.5e-25, 3.1e-28, 1 - eps1)
- #25: ClosePairs NJumps, t = 16: investigate? -- too many unusual p-values (1.4e-4, 9.5e-7, 1 -  3.8e-7)
- #35: Gap, r = 25: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #36: Gap, r = 0: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #37: Gap, r = 20: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #42: Permutation, t = 7: investigate? -- too many unusual p-values (1 -  2.4e-7, 1 -  5.4e-6, 1 -  1.8e-8)
- #43: Permutation, t = 10: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #46: MaxOft, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #47: MaxOft, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #48: MaxOft, t = 24: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #49: MaxOft, t = 32: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #65: SumCollector: investigate? -- too many unusual p-values (1.4e-6, 3.4e-6, 6.3e-7)
- #66: MatrixRank, L=30, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #67: MatrixRank, L=30, r=26: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #68: MatrixRank, L=1000, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #69: MatrixRank, L=1000, r=26: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #70: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #81: LinearComp, r = 29: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #87: LongestHeadRun, r = 27: FAIL!! -- p-values too unlikely (eps, 1 - eps1, eps, 1 - eps1, eps, 1 - eps1)
- #101: Run of bits, r = 0: investigate? -- too many unusual p-values (1 -  1.5e-8, 1 -  3.5e-7, 1 -  4.3e-5)
- #102: Run of bits, r = 27: FAIL!! -- p-values too unlikely (eps, eps, eps)
- 6 unnoteworthy blips (#44, #44, #62, #74, #74, #85)

Summary for splitmix64 lsb 32-bits (byte reverse) (7 crushes):
- #1: SerialOver, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #2: SerialOver, r = 22: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #3: CollisionOver, t = 2: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #4: CollisionOver, t = 2: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #5: CollisionOver, t = 3: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #6: CollisionOver, t = 3: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #7: CollisionOver, t = 7: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #8: CollisionOver, t = 7: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #9: CollisionOver, t = 14: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #10: CollisionOver, t = 14: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #11: CollisionOver, t = 21: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #12: CollisionOver, t = 21: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #13: BirthdaySpacings, t = 2: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #14: BirthdaySpacings, t = 3: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #15: BirthdaySpacings, t = 4: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #16: BirthdaySpacings, t = 7: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #17: BirthdaySpacings, t = 7: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #18: BirthdaySpacings, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #19: BirthdaySpacings, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #20: BirthdaySpacings, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #21: BirthdaySpacings, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #22: ClosePairs mNP2S, t = 3: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps, eps, eps, eps, eps, eps, eps, eps, eps, eps, eps)
- #23: ClosePairs mNP2S, t = 5: FAIL!! -- p-values too unlikely (7.3e-37, eps, eps, eps, eps, eps, 7.3e-37, eps, eps, eps, eps, eps, 7.3e-37, eps, eps, eps, eps, eps)
- #24: ClosePairs mNP2S, t = 9: FAIL!! -- p-values too unlikely (4.4e-18, 1.8e-122, 1.9e-149, 5.8e-34, 5.4e-178, eps, 4.4e-18, 1.8e-122, 1.9e-149, 5.8e-34, 5.4e-178, eps, 4.4e-18, 1.8e-122, 1.9e-149, 5.8e-34, 5.4e-178, eps)
- #25: ClosePairs NJumps, t = 16: investigate? -- too many unusual p-values (0.9993, 0.9993, 0.9993)
- #34: Gap, r = 0: investigate? -- too many unusual p-values (1.1e-9, 1.1e-9, 1.1e-9)
- #35: Gap, r = 25: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #36: Gap, r = 0: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #37: Gap, r = 20: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #42: Permutation, t = 7: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #43: Permutation, t = 10: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #44: CollisionPermut, r = 0: investigate? -- too many unusual p-values (1 -  1.3e-5, 1 -  1.3e-5, 1 -  1.3e-5)
- #46: MaxOft, t = 8: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #47: MaxOft, t = 16: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #48: MaxOft, t = 24: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #49: MaxOft, t = 32: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #66: MatrixRank, L=30, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #67: MatrixRank, L=30, r=26: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #68: MatrixRank, L=1000, r=0: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #69: MatrixRank, L=1000, r=26: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #70: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps)
- #76: RandomWalk1 M (L=1000, r=0): investigate? -- too many unusual p-values (1.3e-5, 1.3e-5, 1.3e-5)
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #81: LinearComp, r = 29: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1, 1 - eps1)
- #84: Fourier3, r = 0: investigate? -- too many unusual p-values (9.6e-4, 9.6e-4, 9.6e-4)
- #101: Run of bits, r = 0: investigate? -- too many unusual p-values (1 -  3.4e-6, 1 -  3.4e-6, 1 -  3.4e-6)
- #102: Run of bits, r = 27: investigate? -- too many unusual p-values (1 -  7.6e-8, 1 -  7.6e-8, 1 -  7.6e-8)
- 1 unnoteworthy blips (#28)

Summary for splitmix64 msb 32-bits (5 crushes):
- 2 unnoteworthy blips (#29, #101)

Summary for splitmix64 msb 32-bits (bit reverse) (3 crushes):
- 1 unnoteworthy blips (#2)

Summary for splitmix64 msb 32-bits (byte reverse) (3 crushes):
- #11: CollisionOver, t = 21: investigate? -- too many unusual p-values (6.5e-4, 6.5e-4, 6.5e-4)

Summary for xoroshiro128plus lsb 32-bits (6 crushes):
- #76: RandomWalk1 M (L=1000, r=0): investigate? -- too many unusual p-values (4.7e-4, 4.7e-4, 4.7e-4)
- 1 unnoteworthy blips (#19)

Summary for xoroshiro128plus lsb 32-bits (bit reverse) (1 crushes):
- #68: MatrixRank, L=1000, r=0: FAIL!! -- p-values too unlikely (eps)
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps)
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1)

Summary for xoroshiro128plus lsb 32-bits (byte reverse) (5 crushes):
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- 2 unnoteworthy blips (#29, #33)

Summary for xoroshiro128plus msb 32-bits (6 crushes):
- #11: CollisionOver, t = 21: investigate? -- too many unusual p-values (0.9996, 0.9996, 0.9996)
- #97: HammingIndep, L=300, r=0: investigate? -- too many unusual p-values (0.9992, 0.9992, 0.9992)
- #98: HammingIndep, L=300, r=26: investigate? -- too many unusual p-values (3.3e-5, 3.3e-5, 3.3e-5)
- 1 unnoteworthy blips (#49)

Summary for xoroshiro128plus msb 32-bits (byte reverse) (5 crushes):
- 1 unnoteworthy blips (#45)

Summary for xorshift128plus lsb 32-bits (5 crushes):
- 3 unnoteworthy blips (#7, #7, #11)

Summary for xorshift128plus lsb 32-bits (bit reverse) (2 crushes):
- #68: MatrixRank, L=1000, r=0: FAIL!! -- p-values too unlikely (eps, eps) -- ALL CRUSHES FAIL!!
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps) -- ALL CRUSHES FAIL!!
- #80: LinearComp, r = 0: FAIL!! -- p-values too unlikely (1 - eps1, 1 - eps1) -- ALL CRUSHES FAIL!!

Summary for xorshift128plus lsb 32-bits (byte reverse) (4 crushes):
- #71: MatrixRank, L=5000: FAIL!! -- p-values too unlikely (eps, eps, eps, eps) -- ALL CRUSHES FAIL!!
- 1 unnoteworthy blips (#97)

Summary for xorshift128plus msb 32-bits (5 crushes):
- 2 unnoteworthy blips (#27, #94)
```

## PractRand results

See practrand/results for detailed outputs.

Raw output:

```
$ ./summarize.sh
testmitchellmoore.log:  [Low1/64]BRank(12):256(2)         R= +73.5  p~=  3.8e-23    FAIL !!
testxoroshiro128plus.log:  [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
testxorshift-k4.log:  BRank(12):256(4)                  R= +5300  p~=  1e-2819    FAIL !!!!!!!!
testxorshift-k5.log:  [Low4/64]BRank(12):768(1)         R=+583.3  p~=  1.2e-176   FAIL !!!!!!
testxorshift128plus-H.log:  BCFN(2+1,13-0,T)                  R= +27.9  p =  1.9e-14    FAIL
testxorshift128plus.log:  [Low4/64]BRank(12):768(1)         R= +1272  p~=  5.4e-384   FAIL !!!!!!!
testxorshift32.log:  BCFN(2+0,13-2,T)                  R=+179.4  p =  2.8e-91    FAIL !!!!!
```



## Speed results

On a recent (Skylake) processor, on a Linux box, I got the following results:

```
aesctr:  1.72 cycles per byte
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
aesctr:  1.72 cycles per byte
pcg32:  1.81 cycles per byte
xorshift32:  2.33 cycles per byte
rand:  5.19 cycles per byte
```

Results will depend on your specific hardware and might be quite different on ARM processors. Tweaking the benchmark could also change the results. In particular, our benchmark stresses throughput as opposed to latency.


## Visual Summary

|                  | TestU01        | PractRand      | time (cycles/byte) |
|------------------|----------------|----------------|--------------------|
| xoroshiro128plus |  fails!  | fails!  | 0.9                |
| splitmix64       |   fails!   |    :+1:        | 0.9                |
| xorshift128plus  | fails! | fails!  | 0.9                |
| pcg64            | possible issue with Run  |    :+1:        | 1.3                |
| pcg32            |   passes   |    :+1:        | 1.8                |
| xorshift32       | fails!  | fails!  | 2.3                |



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
