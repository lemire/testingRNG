
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


.PHONY: clean cleandist

CFLAGS = -fPIC -std=c99 -O3 -march=native -Wall -Wextra -Wshadow -Wno-implicit-function-declaration

EXECUTABLES=testwidynski testlehmer64 testsplitmix64 testxoroshiro128plus testxorshift128plus testxorshift32 testpcg32 testpcg64 testaesctr testmitchellmoore testmersennetwister testxorshift-k4 testxorshift-k5



all: $(EXECUTABLES) ent

random.zip:
	(curl -OL http://www.fourmilab.ch/random/random.zip)

ent: random.zip
	(unzip -qo random.zip -d rand && cd rand &&  make && cp ent ..)

testmitchellmoore: ../practrand/src/testmitchellmoore.c
	$(CC) $(CFLAGS) -o testmitchellmoore ../practrand/src/testmitchellmoore.c -I../source

testmersennetwister: ../practrand/src/testmersennetwister.c
	$(CC) $(CFLAGS) -o testmersennetwister ../practrand/src/testmersennetwister.c -I../source

testxorshift-k4: ../practrand/src/testxorshift-k4.c
	$(CC) $(CFLAGS) -o testxorshift-k4 ../practrand/src/testxorshift-k4.c -I../source

testxorshift-k5: ../practrand/src/testxorshift-k5.c
	$(CC) $(CFLAGS) -o testxorshift-k5 ../practrand/src/testxorshift-k5.c -I../source

testwidynski: ../practrand/src/testwidynski.c
	$(CC) $(CFLAGS) -o testwidynski ../practrand/src/testwidynski.c -I../source

testaesctr: ../practrand/src/testaesctr.c
	$(CC) $(CFLAGS) -o testaesctr ../practrand/src/testaesctr.c -I../source


testlehmer64 : ../practrand/src/testlehmer64.c
	$(CC) $(CFLAGS) -o testlehmer64 ../practrand/src/testlehmer64.c -I../source


testsplitmix64 : ../practrand/src/testsplitmix64.c
	$(CC) $(CFLAGS) -o testsplitmix64 ../practrand/src/testsplitmix64.c -I../source

testxorshift128plus:  ../practrand/src/testxorshift128plus.c
	$(CC) $(CFLAGS) -o testxorshift128plus ../practrand/src/testxorshift128plus.c  -I../source

testxoroshiro128plus:  ../practrand/src/testxoroshiro128plus.c
	$(CC) $(CFLAGS) -o testxoroshiro128plus ../practrand/src/testxoroshiro128plus.c  -I../source

testxorshift32: ../practrand/src/testxorshift32.c
	$(CC) $(CFLAGS) -o testxorshift32 ../practrand/src/testxorshift32.c -I../source

testpcg32: ../practrand/src/testpcg32.c
	$(CC) $(CFLAGS) -o testpcg32 ../practrand/src/testpcg32.c -I../source

testpcg64: ../practrand/src/testpcg64.c
	$(CC) $(CFLAGS) -o testpcg64 ../practrand/src/testpcg64.c -I../source



clean:
	rm -f $(EXECUTABLES) ent

cleandist:
	rm -r -f rand
	rm -f $(EXECUTABLES)
	rm -f ent
