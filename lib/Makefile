DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)
CLU_FILE = ../utils/clu/bin/bin.o

DIR = $(shell basename $(CURDIR))

build: lib.o



lib.o: $(LIB_FILES)
	echo "linking $(DIR) object"
	ld -r -o $@ $^
	
debug.o: $(DGB_FILES) $(CLU_FILE)
	echo "linking $(DIR) debug"
	ld -r -o $@ $^



.PHONY: $(LIB_FILES) $(CLU_FILE)
$(LIB_FILES) $(CLU_FILE):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB_FILES)
$(DGB_FILES):
	$(MAKE) debug.o --directory=$(dir $@)



clean c:
	$(MAKE) _clean -s

_clean:
	echo "cleaning $(DIR)"
	rm -rf *.o
	for DIR in $(DIRS) ; do \
		( $(MAKE) _clean --directory=$$DIR ) || exit $$? ; \
	done
	$(MAKE) clean --directory=$(dir $(CLU_FILE))



.PHONY: test
test t:
	$(MAKE) debug.o
	$(MAKE) _test -s

_test:
	for DIR in $(DIRS) ; do \
		( $(MAKE) _test --directory=$$DIR ) || exit $$? ; \
	done
