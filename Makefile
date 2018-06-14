FLAGS = -g -Wall -Wextra -pedantic
CC = gcc $(FLAGS)

.PHONY: all clean jysh run

all: build

run: build jysh

clean:
	rm -rf build

jysh: build/jysh
	build/jysh

build/jysh: build/main.o build/shell.o
	@$(CC) -o $@ $^

build/%.o: %.c
	$(CC) -o $@ -c $^

build:
	mkdir build
