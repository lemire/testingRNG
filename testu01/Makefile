
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


.PHONY: clean cleandist
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
OSFLAGS= -Wl,--no-as-needed
endif

CFLAGS = -fPIC -std=c99 -O3 -march=native -Wall -Wextra -Wshadow   $(OSFLAGS)

LIBS=build/lib/libmylib.a build/lib/libprobdist.a build/lib/libtestu01.a
LIBTARGET=build/lib/libmylib.a
EXECUTABLES=testsplitmix64 testxoroshiro128plus testxorshift32 testpcg32 testpcg64

HEADERS=include/main.h


all: $(EXECUTABLES)

TestU01.zip:
	(curl -OL http://simul.iro.umontreal.ca/testu01/TestU01.zip)

TestU01-1.2.3: TestU01.zip
	(unzip -qo TestU01.zip)

$(LIBTARGET): TestU01-1.2.3
	(cd TestU01-1.2.3 && ./configure --prefix="$$(dirname "$$(pwd)")/build" && make -j 6 && make -j 6 install)


testsplitmix64 :  $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testsplitmix64 src/testsplitmix64.c $(LIBS) -I../source -Ibuild/include/ -Iinclude

testxoroshiro128plus: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testxoroshiro128plus src/testxoroshiro128plus.c $(LIBS) -I../source -Ibuild/include/ -Iinclude

testxorshift32: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testxorshift32 src/testxorshift32.c $(LIBS) -I../source -Ibuild/include/ -Iinclude

testpcg32: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testpcg32 src/testpcg32.c $(LIBS) -I../source -Ibuild/include/ -Iinclude

testpcg64: $(LIBTARGET) $(HEADERS)
	$(CC) $(CFLAGS) -o testpcg64 src/testpcg64.c $(LIBS) -I../source -Ibuild/include/ -Iinclude



clean:
	rm -f $(EXECUTABLES)

cleandist:
	rm -r -f build
	rm -r -f TestU01-1.2.3
	rm -f $(EXECUTABLES)
