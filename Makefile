SRC = src
LIB = lib
CLU = utils/clu

run:
	$(MAKE) --directory=$(SRC) -s

build b:
	$(MAKE) build --directory=$(SRC)
	
clean c:
	$(MAKE) clean --directory=$(SRC)
	$(MAKE) clean --directory=$(LIB)
	$(MAKE) clean --directory=$(CLU)
	
test t: 
	$(MAKE) test --directory=$(LIB) -s
