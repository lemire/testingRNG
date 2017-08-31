#!/bin/sh

# Download Gists

curl -OL https://gist.github.com/imneme/f76f4bb7b7f67ff0850199ab7c077bf7/raw/832b3469cf157e30419356c1be478eb1049ce602/xoroshiro.hpp
curl -OL https://gist.githubusercontent.com/imneme/9b769cefccac1f2bd728596da3a856dd/raw/a84827604ca8a4b360682292a1f16f526da44191/xorshift.hpp
curl -OL https://gist.githubusercontent.com/imneme/540829265469e673d045/raw/8486a610a954a8248c12485fb4cfc390a5f5f854/randutils.hpp

# Download PCG

curl -o pcg-cpp-master.zip -L https://github.com/imneme/pcg-cpp/archive/master.zip
unzip pcg-cpp-master.zip
