#include <cstdint>
#include <limits>

template<size_t R>
class ChaCha {
public:
    typedef uint32_t result_type;

    explicit ChaCha(uint64_t seedval = 0xfeedfacecafef00d, uint64_t stream = 0);
    template<class Sseq> explicit ChaCha(Sseq& seq);

    void seed(uint64_t seedval, uint64_t stream = 0);
    template<class Sseq> void seed(Sseq& seq);

    uint32_t operator()();
    void discard(unsigned long long n);
    
    template<size_t R_> friend bool operator==(const ChaCha<R_>& lhs, const ChaCha<R_>& rhs);
    template<size_t R_> friend bool operator!=(const ChaCha<R_>& lhs, const ChaCha<R_>& rhs);

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ChaCha<R>& rng);

    template<typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ChaCha<R>& rng);

    static constexpr uint32_t min() { return std::numeric_limits<uint32_t>::min(); }
    static constexpr uint32_t max() { return std::numeric_limits<uint32_t>::max(); }

private:
    void generate_block();
    void chacha_core();

    uint32_t block[16];
    uint32_t keysetup[8];
    uint64_t ctr;
};


template<size_t R>
inline ChaCha<R>::ChaCha(uint64_t seedval, uint64_t stream) {
    seed(seedval, stream);
}

template<size_t R>
template<class Sseq> 
inline ChaCha<R>::ChaCha(Sseq& seq) {
    seedval(seq);
}

template<size_t R>
inline void ChaCha<R>::seed(uint64_t seedval, uint64_t stream) {
    ctr = 0;
    keysetup[0] = seedval & 0xffffffffu;
    keysetup[1] = seedval >> 32;
    keysetup[2] = keysetup[3] = 0xdeadbeef;      // Could use 128-bit seed.
    keysetup[4] = stream & 0xffffffffu;
    keysetup[5] = stream >> 32;
    keysetup[6] = keysetup[7] = 0xdeadbeef;      // Could use 128-bit stream.
}

template<size_t R>
template<class Sseq>
inline void ChaCha<R>::seed(Sseq& seq) {
    ctr = 0;
    seq.generate(keysetup, keysetup + 8);
}


template<size_t R>
inline uint32_t ChaCha<R>::operator()() {
    int idx = ctr % 16;
    if (idx == 0) generate_block();
    ++ctr;

    return block[idx];
}

template<size_t R>
inline void ChaCha<R>::discard(unsigned long long n) {
    int idx = ctr % 16;
    ctr += n;
    if (idx + n >= 16 && ctr % 16 != 0) generate_block();
}

template<size_t R>
inline void ChaCha<R>::generate_block() {
    uint32_t constants[4] = {0x61707865, 0x3320646e, 0x79622d32, 0x6b206574};

    uint32_t input[16];
    for (int i = 0; i < 4; ++i) input[i] = constants[i];
    for (int i = 0; i < 8; ++i) input[4 + i] = keysetup[i];
    input[12] = (ctr / 16) & 0xffffffffu;
    input[13] = (ctr / 16) >> 32;
    input[14] = input[15] = 0xdeadbeef; // Could use 128-bit counter.

    for (int i = 0; i < 16; ++i) block[i] = input[i];
    chacha_core();
    for (int i = 0; i < 16; ++i) block[i] += input[i];
}

#if defined(__SSE2__) && CHACHA_USE_SSE
#include "emmintrin.h"

// Get an efficient _mm_roti_epi32 based on enabled features.
#if !defined(__XOP__)
    #if defined(__SSSE3__)
        #include <tmmintrin.h>
        #define _mm_roti_epi32(r, c) (                              \
            ((c) == 8) ?                                            \
                _mm_shuffle_epi8((r), _mm_set_epi8(14, 13, 12, 15,  \
                                                   10,  9,  8, 11,  \
                                                    6,  5,  4,  7,  \
                                                    2,  1,  0,  3)) \
            : ((c) == 16) ?                                         \
                _mm_shuffle_epi8((r), _mm_set_epi8(13, 12, 15, 14,  \
                                                    9,  8, 11, 10,  \
                                                    5,  4,  7,  6,  \
                                                    1,  0,  3,  2)) \
            : ((c) == 24) ?                                         \
                _mm_shuffle_epi8((r), _mm_set_epi8(12, 15, 14, 13,  \
                                                    8, 11, 10,  9,  \
                                                    4,  7,  6,  5,  \
                                                    0,  3,  2,  1)) \
            :                                                       \
                _mm_xor_si128(_mm_slli_epi32((r), (c)),             \
                              _mm_srli_epi32((r), 32-(c)))          \
        )
    #else
        #define _mm_roti_epi32(r, c) _mm_xor_si128(_mm_slli_epi32((r), (c)), \
                                                   _mm_srli_epi32((r), 32-(c)))
    #endif
#else
    #include <xopintrin.h>
#endif

template<size_t R>
inline void ChaCha<R>::chacha_core() {
    // ROTVn rotates the elements in the given vector n places to the left.
    #define CHACHA_ROTV1(x) _mm_shuffle_epi32((__m128i) x, 0x39)
    #define CHACHA_ROTV2(x) _mm_shuffle_epi32((__m128i) x, 0x4e)
    #define CHACHA_ROTV3(x) _mm_shuffle_epi32((__m128i) x, 0x93)

    __m128i a = _mm_load_si128((__m128i*) (block));
    __m128i b = _mm_load_si128((__m128i*) (block + 4));
    __m128i c = _mm_load_si128((__m128i*) (block + 8));
    __m128i d = _mm_load_si128((__m128i*) (block + 12));

    for (int i = 0; i < R; i += 2) {
        a = _mm_add_epi32(a, b);
        d = _mm_xor_si128(d, a);
        d = _mm_roti_epi32(d, 16);
        c = _mm_add_epi32(c, d);
        b = _mm_xor_si128(b, c);
        b = _mm_roti_epi32(b, 12);
        a = _mm_add_epi32(a, b);
        d = _mm_xor_si128(d, a);
        d = _mm_roti_epi32(d, 8);
        c = _mm_add_epi32(c, d);
        b = _mm_xor_si128(b, c);
        b = _mm_roti_epi32(b, 7);

        b = CHACHA_ROTV1(b);
        c = CHACHA_ROTV2(c);
        d = CHACHA_ROTV3(d);

        a = _mm_add_epi32(a, b);
        d = _mm_xor_si128(d, a);
        d = _mm_roti_epi32(d, 16);
        c = _mm_add_epi32(c, d);
        b = _mm_xor_si128(b, c);
        b = _mm_roti_epi32(b, 12);
        a = _mm_add_epi32(a, b);
        d = _mm_xor_si128(d, a);
        d = _mm_roti_epi32(d, 8);
        c = _mm_add_epi32(c, d);
        b = _mm_xor_si128(b, c);
        b = _mm_roti_epi32(b, 7);

        b = CHACHA_ROTV3(b);
        c = CHACHA_ROTV2(c);
        d = CHACHA_ROTV1(d);
    }

    _mm_store_si128((__m128i*) (block), a);
    _mm_store_si128((__m128i*) (block + 4), b);
    _mm_store_si128((__m128i*) (block + 8), c);
    _mm_store_si128((__m128i*) (block + 12), d);

    #undef CHACHA_ROTV3
    #undef CHACHA_ROTV2
    #undef CHACHA_ROTV1
}
#else
template<size_t R>
inline void ChaCha<R>::chacha_core() {
    #define CHACHA_ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

    #define CHACHA_QUARTERROUND(x, a, b, c, d) \
        x[a] = x[a] + x[b]; x[d] ^= x[a]; x[d] = CHACHA_ROTL32(x[d], 16); \
        x[c] = x[c] + x[d]; x[b] ^= x[c]; x[b] = CHACHA_ROTL32(x[b], 12); \
        x[a] = x[a] + x[b]; x[d] ^= x[a]; x[d] = CHACHA_ROTL32(x[d],  8); \
        x[c] = x[c] + x[d]; x[b] ^= x[c]; x[b] = CHACHA_ROTL32(x[b],  7)

    for (int i = 0; i < R; i += 2) {
        CHACHA_QUARTERROUND(block, 0, 4,  8, 12);
        CHACHA_QUARTERROUND(block, 1, 5,  9, 13);
        CHACHA_QUARTERROUND(block, 2, 6, 10, 14);
        CHACHA_QUARTERROUND(block, 3, 7, 11, 15);
        CHACHA_QUARTERROUND(block, 0, 5, 10, 15);
        CHACHA_QUARTERROUND(block, 1, 6, 11, 12);
        CHACHA_QUARTERROUND(block, 2, 7,  8, 13);
        CHACHA_QUARTERROUND(block, 3, 4,  9, 14);
    }

    #undef CHACHA_QUARTERROUND
    #undef CHACHA_ROTL32
}
#endif


// Implement <random> interface.
template<size_t R>
inline bool operator==(const ChaCha<R>& lhs, const ChaCha<R>& rhs) {
    for (int i = 0; i < 8; ++i) {
        if (lhs.keysetup[i] != rhs.keysetup[i]) return false;
    }

    return lhs.ctr == rhs.ctr;
}

template<size_t R>
inline bool operator!=(const ChaCha<R>& lhs, const ChaCha<R>& rhs) { return !(lhs == rhs); }

template<size_t R, typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const ChaCha<R>& rng) {
    typedef typename std::basic_ostream<CharT, Traits>::ios_base ios_base;

    // Save old state.
    auto flags = os.flags();
    auto fill = os.fill();
    
    // Set flags and fill to space.
    auto space = os.widen(' ');
    os.flags(ios_base::dec | ios_base::fixed | ios_base::left);
    os.fill(space);

    // Serialize.
    for (int i = 0; i < 8; ++i) os << rng.keysetup[i] << space;
    os << rng.ctr;

    // Sestore old state.
    os.flags(flags);
    os.fill(fill);

    return os;
}

template<size_t R, typename CharT, typename Traits>
inline std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, ChaCha<R>& rng) {
    typedef typename std::basic_istream<CharT, Traits> ::ios_base ios_base;

    // Save old flags and set ours.
    auto flags = is.flags();
    is.flags(ios_base::dec);

    // Deserialize.
    for (int i = 0; i < 8; ++i) is >> rng.keysetup[i];
    is >> rng.ctr;

    // Restore old flags.
    is.flags(flags);

    return is;
}