
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


.PHONY: clean cleandist

CFLAGS = -fPIC -std=c99 -O3 -march=native -Wall -Wextra -Wshadow -Wno-implicit-function-declaration

EXECUTABLES=testsplitmix64 testxoroshiro128plus testxorshift128plus testxorshift32 testpcg32 testpcg64 testaesctr

HEADERS=include/main.h


all: $(EXECUTABLES) RNG_test

PractRand_0.93.zip:
	(curl -OL https://downloads.sourceforge.net/project/pracrand/PractRand_0.93.zip)

practrand-0.93-bigbuffer.patch :
	(curl -OL http://www.pcg-random.org/downloads/practrand-0.93-bigbuffer.patch)

RNG_test: practrand-0.93-bigbuffer.patch PractRand_0.93.zip
	(unzip -qo PractRand_0.93.zip -d build && cd build &&  cat ../practrand-0.93-bigbuffer.patch | patch -p0 && $(CXX) -std=c++14 -c src/*.cpp src/RNGs/*.cpp src/RNGs/other/*.cpp -O3 -Iinclude -pthread &&  ar rcs libPractRand.a *.o &&  $(CXX) -std=c++14 -o RNG_test tools/RNG_test.cpp libPractRand.a -O3 -Iinclude -pthread && cp RNG_test ..)

testaesctr: src/testaesctr.c
	$(CC) $(CFLAGS) -o testaesctr src/testaesctr.c -I../source 


testsplitmix64 : src/testsplitmix64.c
	$(CC) $(CFLAGS) -o testsplitmix64 src/testsplitmix64.c -I../source

testxorshift128plus:  src/testxorshift128plus.c
	$(CC) $(CFLAGS) -o testxorshift128plus src/testxorshift128plus.c  -I../source

testxoroshiro128plus:  src/testxoroshiro128plus.c
	$(CC) $(CFLAGS) -o testxoroshiro128plus src/testxoroshiro128plus.c  -I../source

testxorshift32: src/testxorshift32.c
	$(CC) $(CFLAGS) -o testxorshift32 src/testxorshift32.c -I../source

testpcg32: src/testpcg32.c
	$(CC) $(CFLAGS) -o testpcg32 src/testpcg32.c -I../source

testpcg64: src/testpcg64.c
	$(CC) $(CFLAGS) -o testpcg64 src/testpcg64.c -I../source



clean:
	rm -f $(EXECUTABLES) RNG_test

cleandist:
	rm -r -f build
	rm -f $(EXECUTABLES)
	rm -f RNG_test
