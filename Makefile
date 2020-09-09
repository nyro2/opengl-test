CFLAGS=-std=c++17 -Wall --pedantic-errors
LDFLAGS=-lglfw -lpthread -lX11 -ldl

main: main.cpp glad.o
	clang++ $(CFLAGS) -o $@ main.cpp glad.o $(LDFLAGS)

glad.o: glad.c
	clang -c glad.c -o glad.o $(LDFLAGS)

.PHONY: run clean

run: main
	./main

clean:
	rm -rf main main.o
