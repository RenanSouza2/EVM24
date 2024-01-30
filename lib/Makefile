DIRS := $(wildcard */.)

LIB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))lib.o)
DGB_FILES = $(foreach DIR,$(DIRS),$(dir $(DIR))debug.o)
MEM_FILE = ../utils/clu/bin/bin.o

build: lib.o



lib.o: $(LIB_FILES)
	ld -r -o $@ $^
	
debug.o: $(DGB_FILES) $(MEM_FILE)
	ld -r -o $@ $^



.PHONY: $(LIB_FILES)
$(LIB_FILES):
	$(MAKE) --directory=$(dir $@)

.PHONY: $(DGB_FILES)
$(DGB_FILES):
	$(MAKE) debug.o --directory=$(dir $@)

.PHONY: $(MEM_FILE)
$(MEM_FILE):
	$(MAKE) --directory=$(dir $@)



.PHONY: clean c
clean c:
	rm -rf *.o
	for DIR in $(DIRS) ; do \
		( $(MAKE) clean --directory=$$DIR ) || exit $$? ; \
	done

.PHONY: test t
test t:
	for DIR in $(DIRS) ; do \
		( $(MAKE) test --directory=$$DIR ) || exit $$? ; \
	done
