FLAGS = -c -Wall -Werror -Wfatal-errors

LIB_ROOT = $(word 1,$(subst lib, ,$(CURDIR)))lib
DIR = lib$(word 2,$(subst lib, ,$(CURDIR)))

build: lib.o



lib.o: code.c
	echo "building object $(DIR)"
	gcc -o $@ $^ $(FLAGS)

debug.o: code.c
	echo "building debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG



clean c:
	$(MAKE) clean --directory=$(LIB_ROOT) -s

_clean:
	echo "cleaning $(DIR)"
	rm -f *.o
	$(MAKE) clean --directory=test



.PHONY: test
test t:
	$(MAKE) debug.o --directory=$(LIB_ROOT) -s
	$(MAKE) _test -s

_test:
	$(MAKE) --directory=test

export
