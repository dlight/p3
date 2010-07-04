.PHONY: all
.PHONY: run
.PHONY: clean

CC = clang
#CFLAGS = -ggdb

all : prova3

run : all
	./prova3

clean:
	rm prova3 *.o

# $< : primeiro

prova3 : main.o threads.o
	$(CC) $(CFLAGS) `pkg-config --libs gtk+-2.0` -lpthread $^ -o $@

main.o : main.c
	$(CC) $(CFLAGS) -c -DGTK_MULTIHEAD_SAFE=1 `pkg-config --cflags gtk+-2.0` $^ -o $@

threads.o : threads.c
	$(CC) $(CFLAGS) -c $^ -o $@
