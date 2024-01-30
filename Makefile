SRC = src
LIB = lib

run:
	$(MAKE) --directory=$(SRC) 

build b:
	$(MAKE) build --directory=$(SRC)
	
clean c:
	$(MAKE) clean --directory=$(SRC)
	$(MAKE) clean --directory=$(LIB)
	
test t: 
	$(MAKE) test --directory=$(LIB) -s
