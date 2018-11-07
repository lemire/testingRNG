
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


.PHONY: clean cleandist
PROCESSOR:=$(shell uname -m)
ifeq ($(PROCESSOR), aarch64)
OPTIFLAG =
else
OPTIFLAG = -march=native
endif

CFLAGS = -fPIC -std=c99 -O3 $(OPTIFLAG) -Wall -Wextra -Wshadow -Wno-implicit-function-declaration

LIBS=build/lib/libmylib.a build/lib/libprobdist.a build/lib/libtestu01.a
LIBSCMD= build/lib/libtestu01.a  build/lib/libprobdist.a build/lib/libmylib.a  -lm
LIBTARGET=build/lib/libmylib.a
EXECUTABLES=testaesdragontamer testmersennetwister testlehmer64 testsplitmix64 testxoroshiro128plus testv8xorshift128plus testxorshift128plus testxorshift32 testpcg32 testpcg64 testaesctr testxorshift1024star testxorshift1024plus

HEADERS=include/main.h


all: $(EXECUTABLES)

TestU01.zip:
	(curl -OL http://simul.iro.umontreal.ca/testu01/TestU01.zip)

TestU01-1.2.3: TestU01.zip
	(unzip -qo TestU01.zip)

$(LIBTARGET): TestU01-1.2.3
	(cd TestU01-1.2.3 && ./configure --prefix="$$(dirname "$$(pwd)")/build" && make -j 6 && make -j 6 install)

testmersennetwister:  $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testmersennetwister src/testmersennetwister.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testlehmer64:  $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testlehmer64 src/testlehmer64.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testaesctr:  $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testaesctr src/testaesctr.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testaesdragontamer:  $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testaesdragontamer src/testaesdragontamer.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testsplitmix64 :  $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testsplitmix64 src/testsplitmix64.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testxoroshiro128plus: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testxoroshiro128plus src/testxoroshiro128plus.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testxorshift1024star:  $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testxorshift1024star src/testxorshift1024star.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testxorshift1024plus:  $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testxorshift1024plus src/testxorshift1024plus.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testxorshift128plus: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testxorshift128plus src/testxorshift128plus.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude


testv8xorshift128plus: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testv8xorshift128plus src/testv8xorshift128plus.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testxorshift32: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testxorshift32 src/testxorshift32.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testpcg32: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testpcg32 src/testpcg32.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude

testpcg64: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testpcg64 src/testpcg64.c $(LIBSCMD) -I../source -Ibuild/include/ -Iinclude



clean:
	rm -f $(EXECUTABLES)

cleandist:
	rm -r -f build
	rm -r -f TestU01-1.2.3
	rm -f $(EXECUTABLES)
