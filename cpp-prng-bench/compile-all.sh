#!/bin/sh
unzip -oq pcg-cpp-master.zip
CXX="g++ -std=c++14"
# CXX="clang++ -std=c++14"
OPT="-O3 -ggdb -march=native"

for cmd in icksort shuffle micro-bench micro-bench2
do 
    echo Compiling $cmd
    $CXX $OPT $cmd.cpp -o $cmd-mt19937 -DRNG_TYPE=std::mt19937
    $CXX $OPT $cmd.cpp -o $cmd-mt19937_64 -DRNG_TYPE=std::mt19937_64
    $CXX $OPT $cmd.cpp -o $cmd-pcg32 -DINCLUDE_FILE='"pcg_random.hpp"' -DRNG_TYPE=pcg32 -DPCG_USE_INLINE_ASM  -Ipcg-cpp-master/include
    $CXX $OPT $cmd.cpp -o $cmd-pcg32_fast -DINCLUDE_FILE='"pcg_random.hpp"' -DRNG_TYPE=pcg32_fast -DPCG_USE_INLINE_ASM  -Ipcg-cpp-master/include
    $CXX $OPT $cmd.cpp -o $cmd-pcg64 -DINCLUDE_FILE='"pcg_random.hpp"' -DRNG_TYPE=pcg64 -DPCG_USE_INLINE_ASM  -Ipcg-cpp-master/include
    $CXX $OPT $cmd.cpp -o $cmd-pcg64_fast -DINCLUDE_FILE='"pcg_random.hpp"' -DRNG_TYPE=pcg64_fast -DPCG_USE_INLINE_ASM  -Ipcg-cpp-master/include
    $CXX $OPT $cmd.cpp -o $cmd-xoroshiro128plus64 -DINCLUDE_FILE='"xoroshiro.hpp"' -DRNG_TYPE=xoroshiro128plus64
    $CXX $OPT $cmd.cpp -o $cmd-MCG128HighFast -DINCLUDE_FILE='"other_rngs.hpp"' -DRNG_TYPE=MCG128HighFast
    $CXX $OPT $cmd.cpp -o $cmd-PCG64_OnceFast  -DINCLUDE_FILE='"other_rngs.hpp"' -DRNG_TYPE=PCG64_OnceFast
    $CXX $OPT $cmd.cpp -o $cmd-SplitMix  -DINCLUDE_FILE='"other_rngs.hpp"' -DRNG_TYPE=SplitMix
    $CXX $OPT $cmd.cpp -o $cmd-Widynski32  -DINCLUDE_FILE='"other_rngs.hpp"' -DRNG_TYPE=Widynski32
    $CXX $OPT $cmd.cpp -o $cmd-MitchellMoore32  -DINCLUDE_FILE='"other_rngs.hpp"' -DRNG_TYPE=MitchellMoore32
    $CXX $OPT $cmd.cpp -o $cmd-AESCounter64  -DINCLUDE_FILE='"other_rngs.hpp"' -DRNG_TYPE=AESCounter64
    $CXX $OPT $cmd.cpp -o $cmd-Xorshift1024Star64  -DINCLUDE_FILE='"other_rngs.hpp"' -DRNG_TYPE=Xorshift1024Star64
    $CXX $OPT $cmd.cpp -o $cmd-xorshift64star32a  -DINCLUDE_FILE='"xorshift.hpp"' -DRNG_TYPE=xorshift64star32a
    $CXX $OPT $cmd.cpp -o $cmd-arc4_rand  -DINCLUDE_FILE='"arc4_random.hpp"' -DRNG_TYPE=arc4_rand
    $CXX $OPT $cmd.cpp -o $cmd-arc4_rand64  -DINCLUDE_FILE='"arc4_random.hpp"' -DRNG_TYPE=arc4_rand64
    $CXX $OPT $cmd.cpp -o $cmd-chacha3  -DINCLUDE_FILE='"chacha.hpp"' -DRNG_TYPE='ChaCha<3>'
    $CXX $OPT $cmd.cpp -o $cmd-Tychei  -DINCLUDE_FILE='"tychei.hpp"' -DRNG_TYPE='Tychei'
done
