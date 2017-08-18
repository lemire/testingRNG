
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


.PHONY: clean cleandist

CFLAGS = -fPIC -std=c99 -O3 -march=native -Wall -Wextra -Wshadow -Wno-implicit-function-declaration -Wno-incompatible-pointer-types

EXECUTABLES=rng



all: $(EXECUTABLES)
rng : src/rng.c
	$(CC) $(CFLAGS) -o rng src/rng.c -I../source
test:rng
	./rng


clean:
	rm -f $(EXECUTABLES)

cleandist:
	rm -f $(EXECUTABLES)
