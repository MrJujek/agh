CC = gcc
CFLAGS = -std=c2x -O3 -pedantic -Wall -Wextra

all: bin/main bin/worker

bin/main: src/main.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $<

bin/worker: src/worker.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf bin/main bin/worker bin obj
