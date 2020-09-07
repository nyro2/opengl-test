CFLAGS=-std=c++17 -Wall --pedantic-errors
LDFLAGS=-lglfw3 -lpthread -lX11 -ldl

main: main.cpp glad.c
	clang++ $(CFLAGS) -o $@ main.cpp glad.c $(LDFLAGS)

.PHONY: run clean

run: main
	./main

clean:
	rm -rf main main.o
