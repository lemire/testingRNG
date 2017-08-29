#include <cstdint>
#include <limits>
#include <array>

// Implementation of the Tyche-i RNG by Samuel Neves and Filipe Araujo.
// Period is expected to be 2^127, with a very good uniform distribution.
// Can be used as a random engine for the C++11 <random> library.
// WARNING: NOT FOR CRYPTOGRAPHIC USE
class Tychei {
public:
    typedef uint32_t result_type;

    // If no seed is passed to the constructor it will use a platform-specific
    // seeding method, otherwise it will use the given seed. The index argument
    // is optional and safe to ignore, for more information on the use of it see
    // the Tyche-i paper (http://eden.dei.uc.pt/~sneves/pubs/2011-snfa2.pdf).
    explicit Tychei(uint64_t value = 0xfeedfacecafef00d, uint32_t index = 0);
    template<class Sseq> explicit Tychei(Sseq& seq);

    // re-seed the Tychei
    void seed(uint64_t value, uint32_t index = 0);
    template<class Sseq> void seed(Sseq& seq);

    // <random> interface
    uint32_t operator()();
    void discard(unsigned long long n);
    
    friend bool operator==(const Tychei& lhs, const Tychei& rhs);
    friend bool operator!=(const Tychei& lhs, const Tychei& rhs);

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Tychei& rng);

    template<typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, Tychei& rng);

    static constexpr uint32_t min() { return std::numeric_limits<uint32_t>::min(); }
    static constexpr uint32_t max() { return std::numeric_limits<uint32_t>::max(); }

private:
    uint32_t mix();

    uint32_t a, b, c, d;
};
    
    
inline Tychei::Tychei(uint64_t value, uint32_t index) {
    seed(value, index);
}

template<class Sseq> 
inline Tychei::Tychei(Sseq& seq) {
    seed(seq);
}

inline void Tychei::seed(uint64_t value, uint32_t index) {
    a = value;
    b = value >> 32;
    c = 2654435769u;
    d = 1367130551u ^ index;

    for (int i = 0; i < 20; ++i) mix();
}

template<class Sseq>
inline void Tychei::seed(Sseq& seq) {
    std::array<uint32_t, 4> seed_values;
    seq.generate(seed_values.begin(), seed_values.end());

    // We shouldn't need more mixing - the values should be equally distributed.
    a = seed_values[0];
    b = seed_values[1];
    c = seed_values[2];
    d = seed_values[3];
}

inline uint32_t Tychei::mix() {
    #define TYCHEI_ROTL32(x, n) ((x << n) | (x >> (32 - n)))
    b = TYCHEI_ROTL32(b, 25) ^ c;
    d = TYCHEI_ROTL32(d, 24) ^ a;
    c -= d;
    a -= b;
    b = TYCHEI_ROTL32(b, 20) ^ c;
    d = TYCHEI_ROTL32(d, 16) ^ a;
    c -= d;
    a -= b;
    #undef TYCHEI_ROTL32

    return b;
}

uint32_t Tychei::operator()() {
    return mix();
}

void Tychei::discard(unsigned long long n) {
    while (n--) mix();
}

inline bool operator==(const Tychei& lhs, const Tychei& rhs) {
    return lhs.a == rhs.a &&
           lhs.b == rhs.b &&
           lhs.c == rhs.c &&
           lhs.d == rhs.d;
}

inline bool operator!=(const Tychei& lhs, const Tychei& rhs) { return !(lhs == rhs); }

template<typename CharT, typename Traits>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const Tychei& rng) {
    typedef typename std::basic_ostream<CharT, Traits>::ios_base ios_base;

    // Save old state.
    auto flags = os.flags();
    auto fill = os.fill();
    
    // Set flags and fill to space.
    auto space = os.widen(' ');
    os.flags(ios_base::dec | ios_base::fixed | ios_base::left);
    os.fill(space);

    // Serialize.
    os << rng.a << space << rng.b << space << rng.c << space << rng.d;

    // Restore old state.
    os.flags(flags);
    os.fill(fill);

    return os;
}

template<typename CharT, typename Traits>
inline std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, Tychei& rng) {
    typedef typename std::basic_istream<CharT, Traits> ::ios_base ios_base;

    // Save old flags and set ours.
    auto flags = is.flags();
    is.flags(ios_base::dec);

    // Deserialize.
    is >> rng.a >> rng.b >> rng.c >> rng.d;

    // Restore old flags.
    is.flags(flags);

    return is;
}