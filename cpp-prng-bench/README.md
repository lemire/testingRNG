## Test benchmark

Initially produced by Melissa O'Neill (Harvey Mudd).

Build the multiple instances (one per PRNG) of the four benchmark programs:

    ./compile-all.sh

Ensure that your CPU runs at a flat frequency and disable turbo boost (on Intel processors).


# todo

- catalog results
- How do results vary per compiler (clang vs gcc)
- How to results vary per flags (-O2, -O3...)
- Compare O'Neill's implementation with other implementations to validate (important regarding, e.g., the shuffling... maybe compare with straight C implementations)
