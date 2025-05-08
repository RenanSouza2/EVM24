SRC = src
LIB = lib

run:
	$(MAKE) --directory=$(SRC) -s

build b:
	$(MAKE) build --directory=$(SRC) -s
	
clean c:
	$(MAKE) clean --directory=$(SRC) -s
	$(MAKE) clean --directory=$(LIB) -s
	
test t:
	$(MAKE) test --directory=$(LIB) -s
