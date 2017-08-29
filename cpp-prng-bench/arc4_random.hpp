/*
 * Arc4 random number generator for OpenBSD.
 * Copyright 1996 David Mazieres <dm@lcs.mit.edu>.
 *
 * Modification and redistribution in source and binary forms is
 * permitted provided that due credit is given to the author and the
 * OpenBSD project (for instance by leaving this copyright notice
 * intact).
 */

/*
 * This code is derived from section 17.1 of Applied Cryptography,
 * second edition, which describes a stream cipher allegedly
 * compatible with RSA Labs "RC4" cipher (the actual description of
 * which is a trade secret).  The same algorithm is used as a stream
 * cipher called "arcfour" in Tatu Ylonen's ssh package.
 *
 * RC4 is a registered trademark of RSA Laboratories.
 *
 * CODE HISTORY & NOTES:
 *
 * This version is based on arc4random.c included in Apple Libc 549.9.4
 * http://opensource.apple.com/source/Libc/Libc-594.9.4/gen/FreeBSD/arc4random.c
 * At the time of writing, this version of the source is the first one
 * found with a Google search for arc4random.c, even though it is not the
 * most recent iteration.  It was, however, a reasonable starting point for
 * creating a C++ class.
 *
 * Apple's code was a direct import of FreeBSD's 2004/03/24 version of
 * arc4random.c (1.10) (which is available as revision 127373 at
 * https://svnweb.freebsd.org/base/head/lib/libc/gen/arc4random.c), which
 * itself was based the 1997/04/27 version of arc4random.c (1.2) from OpenBSD
 * (http://cvsweb.openbsd.org/cgi-bin/cvsweb/src/lib/libc/crypt/arc4random.c).
 * The FreeBSD version included several small changes made to the OpenBSD
 * source, most notably changes by:
 *      - Christian S.J. Peron <maneo@bsdpro.com> (style, reentrancy)
 *      - Colin Percival <cperciva@freebsd.org> (style)
 *      - Jeff Ito <jeffi@rcn.com> (discard first 1024 bytes, 2004/01/20,
 *        discussed in detail below)
 * (Apple first introduced arc4random in Mac OS X 10.2, released 2002/09/23,
 * which imported FreeBSD's 2000/01/27, revision 1.4 which it kept until
 * OS X 10.5, released 2007/10/26 which is when the above changes, including
 * Ito's change, were first shipped by Apple.)
 *
 * Interesting changes made subseqently to the Apple/FreeBSD code and changes
 * made to other versions:
 *
 * - Amount of output discarded during initialization
 *
 *   On 2008/07/21, Andrey A. Chernov <ache@freebsd.org> reduced the amount
 *   of discarded output to 512 bytes, but then the next day increased it to 768
 *   bytes.  Three days later these changes were backed out, at the request of
 *   Robert Watson  <rwatson@freebsd.org>, restoring the constant to 1024
 *   bytes.
 *
 *   Meanwhile, in the OpenBSD project, Damien Miller <djm@openbsd.org> made
 *   a similar change to Jeff Ito's (in revision 1.10, 2003/11/26, thus
 *   predating Ito's change) but chose to only discard the first 256 bytes of
 *   output. OpenBSD kept using a discard of only 256 bytes until it replaced
 *   Arc4 on 2013/10/01 with ChaCha20 from Nick Mathewson's libottery (an
 *   implementation of Daniel J. Bernstein's Salsa20 stream cipher).  Thus
 *   for over nine years, OpenBSD used a key scheduling scheme that was
 *   considered "too weak" by the FreeBSD developers.
 *
 * - Periodic stirring
 *
 *   In a cryptograpically secure generator, it is wise to periodically
 *   destroy the state of the generator and replace it with an entirely fresh
 *   one.   On 2005/06/04, Ted Unangst <tedu@openbsd.org> added periodic
 *   stirring, causing the generator to reseed itself after producing 40000
 *   outputs.  On 2006/10/04, Andrey A. Chernov <ache@freebsd.org> adopted
 *   this change.  On 2007/02/12, Otto Moerbeek <otto@openbsd.org> changed
 *   the constant to 1600000 without any comment about why (openbsd, r1.15)
 *   and on 2008/07/21 Andrey A. Chernov <ache@freebsd.org> adopted that
 *   change for FreeBSD.
 *
 * - Avoiding *very* short periods
 *
 *   The period of RC4 depends on its initialization.  Some initializations can
 *   cause *very* short periods.  The initial implementation did not correctly
 *   avoid weak initialization, meaning that more than 1 time in 75000, the
 *   period of the generator could be tiny, only 16320 integers
 *   (i.e., 65280 bytes of output).
 *
 *   Theo de Raadt fixed the issue on 1999/09/28 (revision 1.5) but his commit
 *   comment was
 *      "Set j = i after arc4random key schedule to be more like
 *      arc4 stream cipher; from conversations between various people"
 *   which did not give much clue as to the seriousness of the issue, possibly
 *   because he was not aware just how serious it was or because he thought
 *   it unwise to draw attention to the flaw.
 *
 *   The issue it went unnoticed by the FreeBSD developers and was not fixed
 *   until Andrey A. Chernov <ache@freebsd.org> did so on 2008/07/22, almost
 *   nine years later.
 *
 *   Apple did not update its FreeBSD sources until OS X Lion (10.7,
 *   Libc-763.11), which was released on 2011/07/20, almost twelve years
 *   after the flaw was found and fixed by Theo de Raadt.  Presumably the
 *   fix was also applied to iOS 5, which was released that year.
 *
 * This version of the code applies the simple fix for avoiding short periods,
 * and keeps the amount of discarded initial output at 1024 bytes.  It does
 * not provide periodic stirring.  In my opinion, if you want to destroy your
 * RNG state, you should either
 *      - do it manually, after performing a sensitive task, or,
 *      - do it periodically using a timer
 * Also, arbitrary reseeding breaks reproducable behavior, which is required
 * by C++ and by cryptographic uses.
 *
 * This version also changes the initialization setup to use a C++11 seed
 * sequence (or work-alike).  This allows the generator to provide
 * reproducible random numbers.
 *
 * The code is also generalized to allow the number of S-Boxes to be changed.
 * The code can only be used to generate sensible random numbers if the
 * number of boxes is a power of two (controlled by setting the BITS template
 * parameter), but for testing purposes other numbers of boxes can be
 * supported by defining the preprocessor symbol ARC4_OVERRIDE_BOXES to
 * a different value.
 *
 * Note that setting BITS < 6 creates a generator that does *not* pass
 * TestU01's BigCrush test battery (although BITS == 5 is sufficient to pass
 * SmallCrush).
 *
 */

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <cinttypes>
#include <cstring>
#include <limits>
#include <climits>
#include <random>
#include <new>
#include <type_traits>
#include <utility>
#include <iostream>

namespace arc4_detail {

template <typename ResultType = uint32_t, size_t BITS = 8>
class arc4_engine {
#if ARC4_OVERRIDE_BOXES
    static constexpr unsigned int BOXES = ARC4_OVERRIDE_BOXES;
#else
    static constexpr unsigned int BOXES = 1U << BITS;
#endif

    typedef typename std::conditional<BITS <= CHAR_BIT*sizeof(uint8_t),
                                      uint8_t, uint16_t>::type
            box_t;

    box_t i_ = 0;
    box_t j_ = 0;
    box_t s_[BOXES];

public:
    typedef ResultType result_type;

    static constexpr result_type min() {
        return std::numeric_limits<result_type>::min();
    }

    static constexpr size_t boxes() {
        return size_t(BOXES);
    }

    static constexpr result_type max() {
        return std::numeric_limits<result_type>::max();
    }

    arc4_engine(result_type seed = result_type(0xd0beef41U))
        : arc4_engine(std::seed_seq{seed})
    {
        // Nothing else to do.
    }

    template<typename SeedSeq>
    arc4_engine(SeedSeq&& seedSeq, typename std::enable_if<
           !std::is_convertible<SeedSeq, result_type>::value
        && !std::is_convertible<SeedSeq, arc4_engine>::value,
        std::true_type>::type = {});

    template<typename... Args>
    void seed(Args&&... args)
    {
        new (this) arc4_engine(std::forward<Args>(args)...);
    }

    void addrandom(const box_t *dat, size_t datlen);

    box_t getval();

    result_type operator()()
    {
        result_type val = 0;
        constexpr unsigned int RESULT_BITS = sizeof(result_type) * CHAR_BIT;

        for (unsigned int shift = 0; shift < RESULT_BITS; shift += BITS)
            val |= result_type(getval()) << shift;

        return val;
    }

    void discard(size_t t);

    template <typename RT, size_t B>
    friend bool operator==(const arc4_engine<RT,B>&, const arc4_engine<RT,B>&);

    template <typename CharT, typename Traits, typename RT, size_t B>
    friend std::basic_ostream<CharT,Traits>& operator<<(std::basic_ostream<CharT,Traits>&, const arc4_engine<RT,B>&);

    template <typename CharT, typename Traits, typename RT, size_t B>
    friend std::basic_istream<CharT,Traits>& operator>>(std::basic_istream<CharT,Traits>&, arc4_engine<RT,B>&);
};

template <typename ResultType, size_t BITS>
bool operator==(const arc4_engine<ResultType, BITS>& lhs,
                const arc4_engine<ResultType, BITS>& rhs)
{
    return (lhs.i_ == rhs.i_)
         && (lhs.j_ == rhs.j_)
         && !memcmp(lhs.s_, rhs.s_, sizeof(*lhs.i_) * lhs.BOXES);
}

template <typename ResultType, size_t BITS>
inline bool operator!=(const arc4_engine<ResultType, BITS>& lhs,
                       const arc4_engine<ResultType, BITS>& rhs)
{
    return !(lhs == rhs);
}

template <typename CharT, typename Traits, typename ResultType, size_t BITS>
std::basic_ostream<CharT,Traits>&
operator<<(std::basic_ostream<CharT,Traits>& out,
           const arc4_engine<ResultType, BITS>& rng)
{
    auto orig_flags = out.flags(std::ios_base::dec | std::ios_base::left);
    auto space = out.widen(' ');
    auto orig_fill = out.fill();

    out << uint32_t(rng.i_) << space << uint32_t(rng.j_);
    for (auto s : rng.s_)
        out << space << uint32_t(s);

    out.flags(orig_flags);
    out.fill(orig_fill);

    return out;
}

template <typename CharT, typename Traits, typename ResultType, size_t BITS>
std::basic_istream<CharT,Traits>&
operator>>(std::basic_istream<CharT,Traits>& in,
           arc4_engine<ResultType, BITS>& rng)
{
    typename std::basic_istream<CharT,Traits>::sentry s(in);

    if (!s)
         return in;

    uint32_t temp;
    bool fail = false;

    in >> temp;
    fail = temp > rng.boxes();
    if (fail || !in.good())
        goto bail;
    rng.i_ = temp;


    in >> temp;
    fail = temp > rng.boxes();
    if (fail || !in.good())
        goto bail;
    rng.j_ = temp;

    for (auto& s : rng.s_) {
        in >> temp;
        fail = temp > rng.boxes();
        if (fail || !in)
            break;
        s = temp;
    }

bail:
    if (fail)
        in.setstate(std::ios::failbit);

    return in;
}


template <typename ResultType, size_t BITS>
void arc4_engine<ResultType, BITS>::addrandom(const box_t *dat, size_t datlen)
{
    box_t si;

    --i_;
    for (size_t n = 0; n < BOXES; ++n) {
        i_ = (i_ + 1) % BOXES;
        si = s_[i_];
        j_ = (j_ + si + dat[n % datlen]) % BOXES;
        s_[i_] = s_[j_];
        s_[j_] = si;
    }
    j_ = i_;

    /*
     * Throw away the first N bytes of output, as suggested in the
     * paper "Weaknesses in the Key Scheduling Algorithm of RC4"
     * by Fluher, Mantin, and Shamir.  N=1024 is based on
     * suggestions in the paper "(Not So) Random Shuffles of RC4"
     * by Ilya Mironov.
     */
    for (size_t n = 0; n < 1024; ++n)
        getval();
}

template <typename ResultType, size_t BITS>
auto arc4_engine<ResultType, BITS>::getval() -> box_t
{
    box_t si, sj;

    i_ = (i_ + 1) % BOXES;
    si = s_[i_];
    j_ = (j_ + si) % BOXES;
    sj = s_[j_];
    s_[i_] = sj;
    s_[j_] = si;

    return s_[(si + sj) % BOXES];
}


template <typename ResultType, size_t BITS>
void arc4_engine<ResultType, BITS>::discard(size_t amount)
{
    constexpr unsigned int RESULT_BITS = sizeof(result_type) * CHAR_BIT;

    for (size_t i = 0; i < amount; ++i)
        for (unsigned int shift = 0; shift < RESULT_BITS; shift += BITS)
            getval();
}


template <typename ResultType, size_t BITS>
template<typename SeedSeq>
arc4_engine<ResultType, BITS>::arc4_engine(
    SeedSeq&& seedSeq, typename std::enable_if<
           !std::is_convertible<SeedSeq, result_type>::value
        && !std::is_convertible<SeedSeq, arc4_engine>::value,
        std::true_type>::type)
{
    for (size_t n = 0; n < BOXES; ++n)
        s_[n] = n;
    constexpr size_t MAX_POSSIBLE_BOXES = 1U << BITS;
    constexpr bool IS_EFFICIENT = MAX_POSSIBLE_BOXES == BOXES;
    constexpr size_t BOXES_PER_INT32 =
        IS_EFFICIENT ? (sizeof(uint32_t)*CHAR_BIT) / BITS
                     : 1;
    constexpr size_t SEED_BOXES = BOXES/2;
    constexpr size_t SCRIBBLE_SIZE =
        (SEED_BOXES + BOXES_PER_INT32-1) / BOXES_PER_INT32;
    uint32_t scribble[SCRIBBLE_SIZE];
    seedSeq.generate(scribble, scribble+SCRIBBLE_SIZE);
    box_t seeds[SEED_BOXES];
    for (size_t i = 0; i < SEED_BOXES; ++i) {
        seeds[i] =
            (scribble[i / BOXES_PER_INT32] >> (BITS * (i % BOXES_PER_INT32)))
          % BOXES;
    }
    addrandom(seeds, SEED_BOXES);
}

} // namespace arc4_detail

typedef arc4_detail::arc4_engine<uint32_t> arc4_rand;
typedef arc4_detail::arc4_engine<uint64_t> arc4_rand64;
