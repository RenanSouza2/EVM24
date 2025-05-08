FLAGS = -Wall -Wextra -Werror -Wfatal-errors -O3

LIB = ../lib/lib.o

run: main.o
	./main.o

build: main.o

main.o: main.c $(LIB)
	gcc -o $@ $^ $(FLAGS)

.PHONY: $(LIB)
$(LIB):
	$(MAKE) --directory=$(dir $@)

clean:
	rm -rf *.o
