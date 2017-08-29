#ifndef XORSHIFT_HPP_INCLUDED
#define XORSHIFT_HPP_INCLUDED 1

/*
 * A C++ implementation of a family of truncated XorShift* generators.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Melissa E. O'Neill
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <cstdint>

namespace xorshift_detail {

template <typename itype, typename rtype,
          unsigned int a, unsigned int b, unsigned int c>
class xorshift {
protected:
    itype state_;

    static constexpr unsigned int ITYPE_BITS = 8*sizeof(itype);
    static constexpr unsigned int RTYPE_BITS = 8*sizeof(rtype);
    
public:
    using result_type = rtype;
    using state_type = itype;

    static constexpr result_type min() { return 0; }
    static constexpr result_type max() { return result_type(~ result_type(0)); }

    xorshift(itype state = itype(0xc1f651c67c62c6e0))
        : state_(state)
    {
            // Nothing (else) to do.
    }

    void advance()
    {
        state_ ^= state_ >> a;
        state_ ^= state_ << b;
        state_ ^= state_ >> c;
    }
        

    rtype operator()()
    {
        const rtype result = state_;
        advance();
        return result >> (ITYPE_BITS - RTYPE_BITS);
    }

    bool operator==(const xorshift& rhs)
    {
        return state_ == rhs.state_;
    }

    bool operator!=(const xorshift& rhs)
    {
        return !operator==(rhs);
    }

    // Not (yet) implemented:
    //   - arbitrary jumpahead (doable, but annoying to write).
    //   - I/O
    //   - Seeding from a seed_seq.
};

template <typename base, typename rtype, typename base::state_type multiplier>
class xorshiftstar :public base {

    static constexpr unsigned int ITYPE_BITS = 
        8*sizeof(typename base::state_type);
    static constexpr unsigned int RTYPE_BITS = 8*sizeof(rtype);

public:
    using base::base;
    using base::advance;
    using result_type = rtype;

    result_type operator()()
    {
        auto result = base::state_ * multiplier;
        advance();
        return result >> (ITYPE_BITS - RTYPE_BITS);
    }
    
};
} // end of namespace

///// ---- Plain XorShift Generators ---- ////
//
// In the declarations below, the ...plain... versions are pure
// XorShift only, which is known to fail a number of statistical
// tests.  Also, every output only occurs once.  But they will run
// faster, if you are willing to sacrifice quality for speed.
//
// These form the base classes for the XorShift* generators below.
// None of them are intended for general-purpose use, but they may
// be useful in some specialized situations.
//
// Each size has four variations corresponding to different parameter
// sets.  Each will create a distinct (and hopefully statistically
// independent) sequence.
//

// - 128 state bits, __uint128_t output, period 2^128 - 1
// (These constants have been verified as full period.)

using xorshift128plain128a =
    xorshift_detail::xorshift<__uint128_t, __uint128_t, 24, 69, 35>;
using xorshift128plain128b =
    xorshift_detail::xorshift<__uint128_t, __uint128_t, 26, 61, 37>;
using xorshift128plain128c =
    xorshift_detail::xorshift<__uint128_t, __uint128_t, 45, 61, 11>;
using xorshift128plain128d =
    xorshift_detail::xorshift<__uint128_t, __uint128_t, 41, 63, 1>;

// - 64 state bits, uint64_t output, period 2^64 - 1
using xorshift64plain64a =
    xorshift_detail::xorshift<uint64_t, uint64_t, 11, 31, 18>;
using xorshift64plain64b =
    xorshift_detail::xorshift<uint64_t, uint64_t, 11, 29, 14>;
using xorshift64plain64c =
    xorshift_detail::xorshift<uint64_t, uint64_t, 13, 35, 30>;
using xorshift64plain64d =
    xorshift_detail::xorshift<uint64_t, uint64_t, 21, 35, 4>;

// - 32 state bits, uint32_t output, period 2^32 - 1
using xorshift32plain32a =
    xorshift_detail::xorshift<uint32_t, uint32_t, 6, 17, 9>;
using xorshift32plain32b =
    xorshift_detail::xorshift<uint32_t, uint32_t, 5, 17, 13>;
using xorshift32plain32c =
    xorshift_detail::xorshift<uint32_t, uint32_t, 5, 13, 6>;
using xorshift32plain32d =
    xorshift_detail::xorshift<uint32_t, uint32_t, 6, 21, 13>;

        // Note: {5, 21, 12} was tried as a constant, and although it was full
        // period, it had poor statistical results and was retired.

// - 16 state bits, uint16_t output, period 2^16 - 1
using xorshift16plain16a =
    xorshift_detail::xorshift<uint16_t, uint16_t, 5, 9, 8>;
using xorshift16plain16b =
    xorshift_detail::xorshift<uint16_t, uint16_t, 4, 7, 1>;
using xorshift16plain16c =
    xorshift_detail::xorshift<uint16_t, uint16_t, 3, 11, 1>;
using xorshift16plain16d =
    xorshift_detail::xorshift<uint16_t, uint16_t, 7, 9, 13>;

        // Note: {2, 5, 1} was tried as a constant, and although it was full
        // period, it had poor statistical results and was retired.

// - 8 state bits, uint8_t output, period 2^8 - 1
using xorshift8plain8a =
    xorshift_detail::xorshift<uint8_t, uint8_t, 3, 5, 4>;
using xorshift8plain8b =
    xorshift_detail::xorshift<uint8_t, uint8_t, 5, 3, 6>;
using xorshift8plain8c =
    xorshift_detail::xorshift<uint8_t, uint8_t, 5, 3, 1>;
using xorshift8plain8d =
    xorshift_detail::xorshift<uint8_t, uint8_t, 7, 3, 1>;


///// ---- Truncated XorShift* Generators ---- ////
//
// These ...star... versions are truncated XorShift* generators.  They
// should have excellent statistical qualities.  The multiplicative
// constants have been chosen to have good scores under the spectral
// test for LCG multiplicative constants.
//
// Only the XorShift* 128/64 and XorShift* 64/32 generators are
// intended for general-purpose use.  The smaller variants may have
// specialized uses, including exploring the behavior of XorShift* at
// its limits.
//
// Each size has four variations corresponding to different parameter
// sets.  Each will create a distinct (and hopefully statistically
// independent) sequence.
//

// - 128 state bits, uint64_t output, period 2^128 - 1

using xorshift128star64a =
    xorshift_detail::xorshiftstar<xorshift128plain128a, uint64_t, 
                                  0xd1251494eac238a5ULL>;

using xorshift128star64b =
    xorshift_detail::xorshiftstar<xorshift128plain128b, uint64_t, 
                                  0xfa5c5324af636dbdULL>;

using xorshift128star64c =
    xorshift_detail::xorshiftstar<xorshift128plain128c, uint64_t, 
                                  0xe1ee58bdbc73a745ULL>;

using xorshift128star64d =
    xorshift_detail::xorshiftstar<xorshift128plain128d, uint64_t, 
                                  0xf391092cc4e7515dULL>;

// - 64 state bits, uint32_t output, period 2^64 - 1

using xorshift64star32a =
    xorshift_detail::xorshiftstar<xorshift64plain64a, uint32_t, 
                                  0xd989bcacc137dcd5ULL>;
using xorshift64star32b =
    xorshift_detail::xorshiftstar<xorshift64plain64b, uint32_t, 
                                  0xd70ca3edf74bcc9dULL>;
using xorshift64star32c =
    xorshift_detail::xorshiftstar<xorshift64plain64c, uint32_t, 
                                  0x623c3040816c3dcdULL>;
using xorshift64star32d =
    xorshift_detail::xorshiftstar<xorshift64plain64d, uint32_t, 
                                  0x8971baedb6f2fcf5ULL>;

// - 32 state bits, uint16_t output, period 2^32 - 1

using xorshift32star16a =
    xorshift_detail::xorshiftstar<xorshift32plain32a, uint16_t, 0xb2e1cb1dU>;
using xorshift32star16b =
    xorshift_detail::xorshiftstar<xorshift32plain32b, uint16_t, 0x9e02ad0dU>;
using xorshift32star16c =
    xorshift_detail::xorshiftstar<xorshift32plain32c, uint16_t, 0xa6bd8f05U>;
using xorshift32star16d =
    xorshift_detail::xorshiftstar<xorshift32plain32d, uint16_t, 0xbda6a575U>;

// - 16 state bits, uint8_t output, period 2^16 - 1

using xorshift16star8a =
    xorshift_detail::xorshiftstar<xorshift16plain16a, uint8_t, 0x58f5U>;
using xorshift16star8b =
    xorshift_detail::xorshiftstar<xorshift16plain16b, uint8_t, 0xa5e5U>;
using xorshift16star8c =
    xorshift_detail::xorshiftstar<xorshift16plain16c, uint8_t, 0xcbc5U>;
using xorshift16star8d =
    xorshift_detail::xorshiftstar<xorshift16plain16d, uint8_t, 0xc61dU>;

#endif // XORSHIFT_HPP_INCLUDED
