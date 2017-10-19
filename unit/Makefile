
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
