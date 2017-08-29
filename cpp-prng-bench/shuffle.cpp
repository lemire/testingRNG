#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <algorithm>

#ifdef INCLUDE_FILE
#include INCLUDE_FILE
#endif

// stolen from pcg_extras

template <typename RngType>
auto bounded_rand(RngType& rng, typename RngType::result_type upper_bound)
    -> typename RngType::result_type
{
    typedef typename RngType::result_type rtype;
    rtype threshold = (RngType::max() - RngType::min() + rtype(1) - upper_bound)
	% upper_bound;
    for (;;) {
        rtype r = rng() - RngType::min();
        if (r >= threshold)
            return r % upper_bound;
    }
}

template <typename Iter, typename RandType>
void enshuffle(Iter from, Iter to, RandType&& rng)
{
    typedef typename std::iterator_traits<Iter>::difference_type delta_t;
    typedef typename std::remove_reference<RandType>::type::result_type result_t;
    auto count = to - from;
    while (count > 1) {
        delta_t chosen = delta_t(bounded_rand(rng, result_t(count)));
        --count;
        --to;
        using std::swap;
        swap(*(from + chosen), *to);
    }
}

#ifndef RNG_TYPE
#define RNG_TYPE std::mt19937
#endif

#define really_stringify(x) #x
#define stringify(x) really_stringify(x)

int main()
{
    std::random_device rd;
    constexpr unsigned int ITERS = 51;
    double times[ITERS];
    std::cout << "# " stringify(RNG_TYPE) "\n";
    std::cout << "- Shuffling took " << std::flush;
    uint64_t seed = rd();
    seed <<= 32;
    seed |= rd();
    RNG_TYPE rng(seed);
    std::vector<int> vec;
    for (int i = 0; i < 10000000; ++i) {
	vec.push_back(-i);
    }
    for (unsigned int j = 0; j < ITERS; ++j) {
	auto start = std::chrono::high_resolution_clock::now();
	enshuffle(std::begin(vec), std::end(vec), rng);
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = finish-start;
	std::cout << diff.count() << ", " << std::flush;
	times[j] = diff.count();
    }
    std::cout << "... seconds\n";
    std::sort(std::begin(times), std::end(times));
    std::cout << "Median:\t" << times[ITERS/2] << "\n";
    double total = 0.0;
    for (auto v : times) {
	total += v;
    }
    std::cout << "Mean:\t" << (total/ITERS) << "\n";
    return 0;
}

