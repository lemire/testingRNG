
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


.PHONY: clean cleandist

CFLAGS = -fPIC -std=c99 -O3 -march=native -Wall -Wextra -Wshadow -Wno-implicit-function-declaration

EXECUTABLES=v8equiv

all: $(EXECUTABLES)

v8equiv: src/v8equiv.c 
	$(CC) $(CFLAGS) -o v8equiv src/v8equiv.c -I../source
test: v8equiv
	./v8equiv
clean:
	rm -f $(EXECUTABLES)

cleandist:
	rm -f $(EXECUTABLES)
