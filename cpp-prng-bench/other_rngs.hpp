#include <random>
#include <cstring>

struct RNG32Base {
    typedef uint32_t result_type;
    static constexpr result_type min() {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() {
        return std::numeric_limits<result_type>::max();
    }
};

struct RNG64Base {
    typedef uint64_t result_type;
    static constexpr result_type min() {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() {
        return std::numeric_limits<result_type>::max();
    }
};

struct RNG128Base {
    typedef __uint128_t result_type;
    static constexpr result_type min() {
        return 0ULL;
    }
    static constexpr result_type max() {
        return ~(__uint128_t(0ULL));
    }
};

class Times3 : public RNG32Base {
    uint32_t state_;
    static constexpr uint32_t MCG_MULT = 3U;

public:
    Times3(uint32_t state = 0xcafef00d) : state_(state)
    {
        // Nothing else to do.
    }

    uint32_t operator()()
    {
        state_ *= MCG_MULT;
        return state_;
    }
};


class MCG32 : public RNG32Base {
    uint32_t state_;
    static constexpr uint32_t MCG_MULT = 747796405U;

public:
    MCG32(uint32_t state = 0xcafef00d) : state_(state)
    {
        // Nothing else to do.
    }

    uint32_t operator()()
    {
        state_ *= MCG_MULT;
        return state_;
    }
};

class MCG64High : public RNG32Base {
    uint64_t state_;
    static constexpr uint64_t MCG_MULT = 2685821657736338717ULL;

public:
    MCG64High(uint32_t state = 0xcafef00d)
        : state_(0xfeedface00000000ULL | state)
    {
        // Nothing else to do.
    }

    uint32_t operator()()
    {
        state_ *= MCG_MULT;
        return uint32_t(state_ >> 32);
    }
};

class MCG128HighFast : public RNG64Base {
    __uint128_t state_;
    static constexpr uint64_t MCG_MULT = 0xda942042e4dd58b5;

public:
    MCG128HighFast(uint32_t state = 0xcafef00d)
        : state_(0xfeedface00000000ULL | state)
    {
        operator()();
    }

    uint64_t operator()()
    {
        state_ *= MCG_MULT;
        return static_cast<uint64_t>(state_ >> 64);
    }
};


class LCG32 : public RNG32Base {
    uint32_t state_;
    static constexpr uint32_t LCG_MULT1 = 2891336453U;
    static constexpr uint32_t LCG_ADD1  = 24435733U;

public:
    LCG32(uint32_t state = 0xcafef00d) : state_(state)
    {
        // Nothing else to do.
    }

    uint32_t operator()()
    {
        state_ = state_*LCG_MULT1 + LCG_ADD1;
        return state_;
    }
};

class LCG32Hash : public RNG32Base {
    uint32_t state_;
    static constexpr uint32_t LCG_MULT = 2891336453U;
    static constexpr uint32_t LCG_ADD  = 24435733U;

public:
    LCG32Hash(uint32_t state = 0xcafef00d) : state_(state)
    {
        // Nothing else to do.
    }

    uint32_t operator()()
    {
        uint32_t result = state_;
        state_ = state_*LCG_MULT + LCG_ADD;
        result *= 741103597U;
        result = (result << 8) | (result >> 24);
        result *= 1597334677U;
        return result;
    }
};

class LCG32FHash : public RNG32Base {
    uint32_t state_;
    static constexpr uint32_t LCG_MULT = 2891336453U;
    static constexpr uint32_t LCG_ADD  = 24435733U;

public:
    LCG32FHash(uint32_t state = 0xcafef00d) : state_(state)
    {
        // Nothing else to do.
    }

    uint32_t operator()()
    {
        uint32_t result = state_;
        state_ = state_*LCG_MULT + LCG_ADD;
        result ^= result >> 11;
        result *= 0x2127599ULL;
        result ^= result >> 23;
        return result;
    }
};

class LCG32FHash3 : public RNG32Base {
    uint32_t state_;
    static constexpr uint32_t LCG_MULT = 2891336453U;
    static constexpr uint32_t LCG_ADD  = 24435733U;

public:
    LCG32FHash3(uint32_t state = 0xcafef00d) : state_(state)
    {
        // Nothing else to do.
    }

    uint32_t operator()()
    {
        uint32_t result = state_;
        state_ = state_*LCG_MULT + LCG_ADD;
        result ^= result >> 11;
        result *= 747796405U;
        result ^= result >> 23;
        result *= 1597334677U;
        result ^= result >> 12;
        result *= 747796405U;
        result ^= result >> 24;
        return result;
    }
};

class MCG128High : public RNG64Base {
    __uint128_t state_;
    static constexpr __uint128_t MCG_MULT =
        (__uint128_t(5017888479014934897ULL) << 64) + 2747143273072462557ULL;

public:
    MCG128High(uint32_t state = 0xcafef00d)
        : state_(0xfeedface00000000ULL | state)
    {
        operator()();
    }

    uint64_t operator()()
    {
        uint8_t shift = 64;
        state_ = state_*MCG_MULT;
        return static_cast<uint64_t>(state_ >> shift);
    }
};

class LCG128 : public RNG128Base {
    __uint128_t state_;
    static constexpr __uint128_t MULT =
        (__uint128_t(2549297995355413924ULL) << 64) + 4865540595714422341ULL;
    static constexpr __uint128_t ADD =
        (__uint128_t(6364136223846793005ULL) << 64) + 1442695040888963407ULL;

public:
    LCG128(uint32_t state = 0xcafef00d)
        : state_(0xfeedface00000000ULL | state)
    {
        operator()();
    }

    result_type operator()()
    {
        state_ = state_ * MULT + ADD;
        return state_;
    }
};


class RanQ1 : public RNG64Base {
    uint64_t state_;

public:
    RanQ1(uint32_t state = 0xcafef00d)
        : state_(4101842887655102017ULL ^ 0xcafef00d ^ state)
    {
        operator()();
    }

    uint64_t operator()()
    {
        state_ ^= state_ >> 21;
        state_ ^= state_ << 35;
        state_ ^= state_ >> 4;

        return state_ * 2685821657736338717ULL;
    }
};

class RanQ1_32 : public RNG32Base {
    uint32_t state_;

public:
    RanQ1_32(uint32_t state = 0xcafef00d)
        : state_(state)
    {
        operator()();
    }

    uint32_t operator()()
    {
        state_ ^= state_ >> 13;
        state_ ^= state_ << 17;
        state_ ^= state_ >> 5;

        return state_ * 747796405U;
    }
};


struct SplitMix : public RNG64Base {

    uint64_t s = 0xfeedfacecafef00d;

    SplitMix() = default;

    SplitMix(uint32_t state) {
        s ^= state;
    }

    uint64_t operator()() { 
	uint64_t z = ( s += 0x9E3779B97F4A7C15ULL );
        asm("# Prevent conflation with loop variable" : "+r" (s));
	z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
	z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
	return z ^ (z >> 31);
    }
};

struct PCG64Faster : public RNG64Base {

    uint64_t s = 0xfeedfacecafef00d;
    uint64_t strm = 0x9E3779B97F4A7C15ULL;

    PCG64Faster() = default;

    PCG64Faster(uint32_t state) {
        s ^= state;
    }

    uint64_t operator()() { 
        strm += s;
        uint64_t z = strm;
        s *= 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        uint64_t result = z ^ (z >> 31);
        return result;
    }
};

struct PCG64_OnceFast : public RNG64Base {

    uint64_t s = 0xfeedfacecafef00d;

    PCG64_OnceFast() = default;

    PCG64_OnceFast(uint32_t state) {
        s ^= state;
    }

    uint64_t operator()() { 
	uint64_t z = s;
	s *= 0xBF58476D1CE4E5B9ULL;
	s += 0x9E3779B97F4A7C15ULL;
	z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
	return z ^ (z >> 31);
    }
};


struct Ran : public RNG64Base {
    uint64_t u = 0xf5ae80548be1c935;        // Based on init with
    uint64_t v = 0xdecbabc07c6f66bb;        // 0xfeedfacecafef00dULL
    uint64_t w = 0xa876c6ca0eff65e2;
    
    Ran() = default;
        
    Ran(uint64_t j) 
        : u(0), v(4101842887655102017ULL), w(1)
    {
        u = j ^ v;
        operator()();
        v = u;
        operator()();
        w = v;
        operator()();
    }
    
    int64_t operator()()
    {
        u = u * 2862933555777941757ULL + 7046029254386353087ULL;
        v ^= v >> 17;
        v ^= v << 31;
        v ^= v >> 8;
        w = 4294957665U*(w & 0xffffffff) + (w >> 32);
        uint64_t x = u ^ (u << 21);
        x ^= x >> 35;
        x ^= x << 4;
        return (x + v) ^ w;
    }
};

// based on https://arxiv.org/pdf/1704.00358.pdf

class Widynski32 : public RNG32Base {
    uint64_t g_widynski_x;
    uint64_t g_widynski_w;
    uint64_t g_widynski_s;

public:
    Widynski32(uint64_t seed) {
	g_widynski_w = 0;
	g_widynski_x = 0;
	g_widynski_s = seed;
	g_widynski_s |= 1;
	if ((g_widynski_s >> 32) == 0)
	    g_widynski_s = g_widynski_s | (g_widynski_s << 32);
    }

    uint32_t operator()() {
	g_widynski_x *= g_widynski_x;
	g_widynski_x += (g_widynski_w += g_widynski_s);
	g_widynski_x  = (g_widynski_x >> 32) | ( g_widynski_x << 32);
	return g_widynski_x;
    }
};

class MitchellMoore32 : public RNG32Base {
    static constexpr size_t R = 250;
    static constexpr size_t S = 200;
    static constexpr size_t T = 103;
    static constexpr size_t U = 50;

    uint32_t mitchellmoore_sequence[R];
    uint32_t mitchellmoore_a = R, mitchellmoore_b = S,
	mitchellmoore_c = T, mitchellmoore_d = U;

public:    
    MitchellMoore32(uint64_t seed) {
	for (size_t i = 0; i < R * 2; ++i)
	    mitchellmoore_sequence[i % R] = seed = (1664525 * seed + 1013904223);

	mitchellmoore_sequence[0] <<= 1;
	mitchellmoore_sequence[1] |= 1;
    }

    uint32_t operator()() {
	return mitchellmoore_sequence[++mitchellmoore_a % R] +=
	    mitchellmoore_sequence[++mitchellmoore_b % R] +=
	    mitchellmoore_sequence[++mitchellmoore_c % R] +=
	    mitchellmoore_sequence[++mitchellmoore_d % R];
    }
};

#include <immintrin.h>

class AESCounter64 : public RNG64Base {
    static constexpr size_t AESCTR_UNROLL = 4;
    static constexpr size_t AESCTR_ROUNDS = 10;

    uint8_t state[16 * AESCTR_UNROLL];
    __m128i ctr[AESCTR_UNROLL];
    __m128i seed[AESCTR_ROUNDS + 1];
    size_t offset;

#define AES_ROUND(rcon, index)                                                \
    do {                                                                      \
	__m128i k2 = _mm_aeskeygenassist_si128(k, rcon);                      \
	k = _mm_xor_si128(k, _mm_slli_si128(k, 4));                           \
	k = _mm_xor_si128(k, _mm_slli_si128(k, 4));                           \
	k = _mm_xor_si128(k, _mm_slli_si128(k, 4));                           \
	k = _mm_xor_si128(k, _mm_shuffle_epi32(k2, _MM_SHUFFLE(3, 3, 3, 3))); \
	this->seed[index] = k;                                                \
    } while (0)

public:

    AESCounter64(uint64_t seed) {
	__m128i k = _mm_set_epi64x(0, seed);
	this->seed[0] = k;

	AES_ROUND(0x8d, 0);
	AES_ROUND(0x01, 1);
	AES_ROUND(0x02, 2);
	AES_ROUND(0x04, 3);
	AES_ROUND(0x08, 4);
	AES_ROUND(0x10, 5);
	AES_ROUND(0x20, 6);
	AES_ROUND(0x40, 7);
	AES_ROUND(0x80, 8);
	AES_ROUND(0x1b, 9);
	AES_ROUND(0x36, 10);

	for (int i = 0; i < AESCTR_UNROLL; ++i) {
	    this->ctr[i] = _mm_set_epi64x(0, i);
	}
	this->offset = 16 * AESCTR_UNROLL;
    }
    
    uint64_t operator()() {
	if (__builtin_expect(this->offset >= 16 * AESCTR_UNROLL, 0)) {
	    __m128i work[AESCTR_UNROLL];
	    for (int i = 0; i < AESCTR_UNROLL; ++i) {
		work[i] = _mm_xor_si128(this->ctr[i], this->seed[0]);
	    }
	    for (int r = 1; r <= AESCTR_ROUNDS - 1; ++r) {
		const __m128i subkey = this->seed[r];
		for (int i = 0; i < AESCTR_UNROLL; ++i) {
		    work[i] = _mm_aesenc_si128(work[i], subkey);
		}
	    }
	    for (int i = 0; i < AESCTR_UNROLL; ++i) {
		this->ctr[i] =
		    _mm_add_epi64(this->ctr[i], _mm_set_epi64x(0, AESCTR_UNROLL));
		_mm_storeu_si128(
		    (__m128i *)&this->state[16 * i],
		    _mm_aesenclast_si128(work[i], this->seed[AESCTR_ROUNDS]));
	    }
	    this->offset = 0;
	}
	uint64_t output = 0;
	memcpy(&output, &this->state[this->offset], sizeof(output));
	this->offset += sizeof(output);
	return output;
    }

    #undef AES_ROUND
};

class Xorshift1024Star64 : public RNG64Base {
    uint64_t s[16];
    int p = 0;

public:
    Xorshift1024Star64(uint64_t seed) {
	for (auto& v : s) {
	    v = seed;
	    seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
	}
    }

    uint64_t operator()() {
	const uint64_t s0 = s[p];
	uint64_t s1 = s[p = (p + 1) & 15];
	s1 ^= s1 << 31; // a
	s[p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b,c
	return s[p] * UINT64_C(1181783497276652981);
    }
};

class NullRNG : public RNG32Base {

public:
    NullRNG(uint32_t) {}

    uint32_t operator()()
    {
        uint32_t result = 42;
        asm volatile("# NullRNG" : "+r" (result));
        return result;
    }
};

struct BobJenkinsRNG : public RNG32Base {
    uint32_t a,b,c,d;

    #define rot(x,k) (((x)<<(k))|((x)>>(32-(k))))
    uint32_t operator()()
    {
        uint32_t e = a - rot(b, 27);
        a = b ^ rot(c, 17);
        b = c + d;
        c = d + e;
        d = e + a;
        return d;
    }
    
    BobJenkinsRNG(uint32_t seed = 0xcafebabeu)
    {
        uint32_t i;
        a = 0xf1ea5eedu, b = c = d = seed;
        for (i=0; i<20; ++i) {
            (void)operator()();
        }
    }
};
