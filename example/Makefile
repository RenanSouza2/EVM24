FLAGS = -c -Wall -Werror -Wfatal-errors

LIB_ROOT = $(word 1,$(subst lib, ,$(CURDIR)))lib
DIR = $(shell basename $(CURDIR))

build: lib.o



lib.o: code.c
	echo "building clu object $(DIR)"
	gcc -o $@ $^ $(FLAGS)

debug.o: code.c
	echo "building clu debug $(DIR)"
	gcc -o $@ $^ $(FLAGS) -D DEBUG



clean c:
	$(MAKE) clean --directory=$(LIB_ROOT) -s

_clean:
	echo "cleaning clu $(DIR)"
	rm -f *.o
	$(MAKE) clean --directory=test



.PHONY: test
test t:
	$(MAKE) debug.o --directory=$(LIB_ROOT) -s
	$(MAKE) _test -s

_test:
	$(MAKE) --directory=test

export
