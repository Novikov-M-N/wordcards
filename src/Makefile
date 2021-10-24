CC=gcc
CFLAGS=-Wall -Werror -Wextra -Wpedantic

all: main

main: main.c memory.c
	$(CC) $(CFLAGS) $^ -o ../build/wcards

messy: main.c memory.c
	$(CC) $^ -o ../build/messy

debug: main.c memory.c
	$(CC) -DDEBUG=1 $^ -o ../build/debug

clean:
	rm ../build/*

rebuild: clean all