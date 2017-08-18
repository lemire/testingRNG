# testingRNG

There are several benchmarks that can be used to test (pseudo-)random number
generators (RNG). Of particular interest are TestU01 and PractRand. We want to easily
test popular RNGs.

It is fine to say that we can, in theory run these tests, but few of us will if
it requires too much labor. The purpose of this project is to make it ridiculously
easy to run your own tests if you have a mac or a Linux box with a recent C compiler.

## Prerequisites

We assume Linux or macOS. 

- Our scripts should work out of the box on  most Linux boxes, except maybe when they are configured to act as secure servers (e.g., without a C compiler).
- If you have a mac, it is assumed that [you have installed a recent Xcode package](https://developer.apple.com/xcode/) to get
the C (and C++) compiler. Make sure you have installed the command-line utilities.

## Usage

You can run the tests by going to a bash shell (Terminal) and executing a few commands.

PractRand:
```
cd practrand
./runtests.sh
```

The PractRand benchmark is completed quickly.

TestU01:
```
cd testu01
./linearcomplexity.sh
./bigcrush.sh
```

The TestU01 benchmark "big crush" (``bigcrush.sh``) might take days whereas
the linear complexity test (``linearcomplexity.sh``), being narrow, completes quickly.


## Interpreting the results

Tests are subject to both false positives and false negatives, and should be interpreted with care.

- That your random number generator passes several statistical tests does not prove that it is of high quality. There might be other tests that it would fail. However, the more tests one passes, the greater the evidence is in favor of your random number generator, evidently.
- A statistical test might fail even if the bits are indeed "random". Thus a fialed test is not "proof" that the random number generation is faulty. However, failing tests can be further investigated and evidence of a fault can be ascertained.

One source of concern is that random number generators are initialized with a seed, and sometimes a "sequence" is selected. Testing successfully with a given seed does not preclude the possibility that there might be "bad seeds". 

To summarize, caution is required when interpreting the results. It is not black and white, good and bad... One might say that a given generator passes a given test, but it is possible that with different seeds, it could fail, and so forth.

Still, for convenience, it is necessary to express results in a comprehensible manner. Thus we often say that a generator "passes a given test" or does not.

## Testing frameworks

We use the following testing framework:

- [Practically Random](https://sourceforge.net/projects/pracrand/)
- [TestU01](http://simul.iro.umontreal.ca/testu01/)

As of 2017, these represent the state-of-the-art.

## TestU01 Methodology

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

For PractRand, we do not need to truncate the produced random bits.

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
