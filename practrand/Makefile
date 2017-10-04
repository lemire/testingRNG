
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


.PHONY: clean cleandist

CFLAGS = -fPIC -std=c99 -O3 -march=native -Wall -Wextra -Wshadow -Wno-implicit-function-declaration

EXECUTABLES=testwidynski testlehmer64 testsplitmix64  testv8xorshift128plus testxoroshiro128plus testxorshift128plus testxorshift32 testpcg32 testpcg64 testaesctr testmitchellmoore testmersennetwister testxorshift-k4 testxorshift-k5 testxorshift1024star

HEADERS=include/main.h


all: $(EXECUTABLES) RNG_test

PractRand_0.93.zip:
	(curl -OL https://downloads.sourceforge.net/project/pracrand/PractRand_0.93.zip)

practrand-0.93-bigbuffer.patch :
	(curl -OL http://www.pcg-random.org/downloads/practrand-0.93-bigbuffer.patch)

RNG_test: practrand-0.93-bigbuffer.patch PractRand_0.93.zip
	(unzip -qo PractRand_0.93.zip -d build && cd build &&  cat ../practrand-0.93-bigbuffer.patch | patch -p0 && $(CXX) -std=c++14 -c src/*.cpp src/RNGs/*.cpp src/RNGs/other/*.cpp -O3 -Iinclude -pthread &&  ar rcs libPractRand.a *.o &&  $(CXX) -std=c++14 -o RNG_test tools/RNG_test.cpp libPractRand.a -O3 -Iinclude -pthread && cp RNG_test ..)

testmitchellmoore: src/testmitchellmoore.c
	$(CC) $(CFLAGS) -o testmitchellmoore src/testmitchellmoore.c -I../source

testmersennetwister: src/testmersennetwister.c
	$(CC) $(CFLAGS) -o testmersennetwister src/testmersennetwister.c -I../source

testxorshift-k4: src/testxorshift-k4.c
	$(CC) $(CFLAGS) -o testxorshift-k4 src/testxorshift-k4.c -I../source

testxorshift-k5: src/testxorshift-k5.c
	$(CC) $(CFLAGS) -o testxorshift-k5 src/testxorshift-k5.c -I../source


testwidynski: src/testwidynski.c
	$(CC) $(CFLAGS) -o testwidynski src/testwidynski.c -I../source

testaesctr: src/testaesctr.c
	$(CC) $(CFLAGS) -o testaesctr src/testaesctr.c -I../source


testlehmer64 : src/testlehmer64.c
	$(CC) $(CFLAGS) -o testlehmer64 src/testlehmer64.c -I../source


testsplitmix64 : src/testsplitmix64.c
	$(CC) $(CFLAGS) -o testsplitmix64 src/testsplitmix64.c -I../source

testxorshift1024star:  src/testxorshift1024star.c
	$(CC) $(CFLAGS) -o testxorshift1024star src/testxorshift1024star.c  -I../source


testv8xorshift128plus:  src/testv8xorshift128plus.c
	$(CC) $(CFLAGS) -o testv8xorshift128plus src/testv8xorshift128plus.c  -I../source


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
