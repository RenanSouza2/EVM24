DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)

build: lib.o



lib.o debug.o:
	touch $@ 

.PHONY: clean c
clean c:
	rm -rf *.o
	for DIR in $(DIRS) ; do \
		( $(MAKE) clean --directory=$$DIR ) || exit $$? ; \
	done

.PHONY: test t
test t:
