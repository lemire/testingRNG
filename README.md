# testingRNG
Testing common random-number-generators

There are several benchmarks that can be used to test (pseudo-)random number
generators. Of particular interest are TestU01 and PractRand.

## Usage

We assume Linux or macOS.


PractRand:
```
cd practrand
./runtests.sh
```

The PractRand benchmark is completed quickly.

TestU01:
```
cd testu01
./runtests.sh
```

The TestU01 benchmark might take days.




## Testing frameworks

- [Practically Random](https://sourceforge.net/projects/pracrand/)
- [TestU01](http://simul.iro.umontreal.ca/testu01/)


## Methodology

For historical reasons, it appears that TestU01 is designed to test 32-bit numbers
whereas many modern random number generators produce 64-bit numbers. In such cases,
a sensible approach is to "cast" the result of the 64-bit test to a 32-bit integer.
(In C/C++, we do ``uint32_t x32 = (uint32_t) x;``.)
However, as pointed out by [Vigna (2016)](https://arxiv.org/pdf/1402.6246.pdf), we should
make sure that permuting the bit order does not lead to a test failure.

## Talks

- [Stanford Seminar - PCG: A Family of Better Random Number Generators](https://www.youtube.com/watch?v=45Oet5qjlms) (by O'Neill)

## Links of interest (technical)

- [PRNG shootout](http://xoroshiro.di.unimi.it) by Vigna
- How to test PCG with  [PractRand](http://www.pcg-random.org/posts/how-to-test-with-practrand.html) and [TestU01](http://www.pcg-random.org/posts/how-to-test-with-testu01.html) (by O'Neill)

## Blog posts

- [On Melissa O’Neill’s PCG random number generator](http://lemire.me/blog/2017/08/15/on-melissa-oneills-pcg-random-number-generator/)
- [Testing RNGs with PractRand](https://www.johndcook.com/blog/2017/08/14/testing-rngs-with-practrand/), [Manipulating a random number generator](https://www.johndcook.com/blog/2017/08/16/manipulating-a-random-number-generator/) by Cook
- [PCG Passes PractRand](http://www.pcg-random.org/posts/pcg-passes-practrand.html)

## More reading

> A recent example shows the importance of testing the reverse generator. Saito and Matsumoto [2014] propose a different way to eliminate linear artifacts: instead of multiplying the output of an underlying xorshift generator (with 128 bits of state and based on 32-bit shifts) by a constant, they add it (in Z/232Z) with the previous output. Since the sum in Z/232Z is not linear over Z/2Z, the result should be free of linear artifacts. However, while their generator passes BigCrush, its reverse fails systematically the LinearComp, MatrixRank, MaxOft and Permutation test of BigCrush, which highlights a significant weakness in its lower bits. [Vigna, An experimental exploration of Marsaglia’s xorshift generators, scrambled, ACM Transactions on Mathematical Software (TOMS), 2016 ](https://arxiv.org/pdf/1402.6246.pdf)

> xoroshiro128plus seems like an interesting variation on LFSRs, it maintains the excessive linearity problem, but the variation in structure (compared to classic LFSRs) costs little and may significantly improve quality, not sure yet. In PractRand it quickly fails binary rank tests, and also eventually fails a short-medium range linear tests in the expanded test suite (DC6-5x4Byte-1). The author suggests avoiding reliance on the lowest bit to work around the binary rank problem, but the 2nd lowest bit also fails binary rank tests (though it takes substantially larger matrices to detect that). A non-linear output function could fix that stuff, but I'd also prefer a larger statespace. It's not clear how well this PRNG supports random access - a function to skip forward exactly 2^64 is provided, but there's no obvious way to skip forward/backward other amounts - I'd guess it's possible, but requires significant precomputation on a per-power-of-2 basis and might be slow at runtime for distances that are not neat powers of two. I think xoroshiro has a single cycle of length 2^128-1.
[Chris Doty-Humphrey, 2016-09-07](https://sourceforge.net/p/pracrand/discussion/366935/thread/2b43b548/#a33d)
