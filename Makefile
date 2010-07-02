.PHONY: all
.PHONY: run
.PHONY: clean

CC = gcc
CFLAGS = -DGTK_MULTIHEAD_SAFE=1

all : prova3

run : all
	./prova3

clean:
	rm prova3

# $< : primeiro

prova3 : main.o threads.o
	$(CC) `pkg-config --libs gtk+-2.0` -lpthread $^ -o $@

main.o : main.c
	$(CC) -c -DGTK_MULTIHEAD_SAFE=1 `pkg-config --cflags gtk+-2.0` $^ -o $@

threads.o : threads.c
	$(CC) -c $^ -o $@
