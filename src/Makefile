CC=gcc
CFLAGS=-Wall -Werror -Wextra -Wpedantic

all: main

main: main.c words.c
	$(CC) $(CFLAGS) $^ -o ../build/wrun

messy: main.c words.c
	$(CC) $^ -o ../build/messy

debug: main.c words.c
	$(CC) -DDEBUG=1 $^ -o ../build/debug

clean:
	rm ../build/*

rebuild: clean all