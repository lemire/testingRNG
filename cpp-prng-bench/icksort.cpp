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


// Note that last is the last element, not "past the end" point

template <typename Iter, typename RNG>
Iter qs_partition(Iter begin, Iter end, RNG& rng);

template <typename Iter, typename RNG>
__attribute__((noinline))
void quicksort(Iter begin, Iter end, RNG& rng)
{
    // Base case: arrays of size zero and one are already sorted, by definition
    if (end - begin < 2)
	return;

    Iter mid = qs_partition(begin, end, rng);

    quicksort(begin, mid, rng);
    ++mid;
    quicksort(mid, end, rng);
}

template <typename Iter, typename RNG>
Iter qs_partition(Iter begin, Iter end, RNG& rng)
{
    using std::swap;

    // Pick an element to be pivot, move it to the end.
    Iter randomItem = begin + bounded_rand(rng, end - begin);
    --end;
    swap(*randomItem, *end);

    auto& pivot = *end;
    
    // Actually do the partition.
    Iter i = begin;
    Iter j = end;
    --j;
    for (;;) { 
        while (*i < pivot)
            ++i;
        while (*j > *i && pivot < *j)
            --j;
        if (i >= j)
            break;
        swap(*i++, *j--);
    }
    swap(*i, pivot);
    return i;
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
    std::cout << "- Sorting took " << std::flush;
    for (unsigned int j = 0; j < ITERS; ++j) {
	uint64_t seed = rd();
	seed <<= 32;
	seed |= rd();
	RNG_TYPE rng(seed);
	std::vector<int> vec;
	for (int i = 0; i < 10000000; ++i) {
	    vec.push_back(-i);
	}
	auto start = std::chrono::high_resolution_clock::now();
	quicksort(std::begin(vec), std::end(vec), rng);
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

