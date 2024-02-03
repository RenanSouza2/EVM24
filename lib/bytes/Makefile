FLAGS = -c -Wall -Werror -Wfatal-errors

LIB_ROOT = ..
DIR = $(shell basename $(CURDIR))

build: lib.o

lib.o: code.c
	echo "Building $(DIR) object"
	gcc -o $@ $^ $(FLAGS)

debug.o: debug_core.o debug_debug.o
	echo "Linking $(DIR) debug"
	ld -r -o $@ $^

debug_core.o: code.c
	echo "Building $(DIR) debug core"
	gcc -o $@ $^ $(FLAGS) -D DEBUG

debug_debug.o: debug.c
	echo "Building $(DIR) debug core"
	gcc -o $@ $^ $(FLAGS) -D DEBUG

clean c:
	rm -f *.o
	$(MAKE) clean --directory=test

.PHONY: test t
test t:
	$(MAKE) --directory=test

export
