CC=gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -lm
DEPS = gol.h

.PHONY : all
all : gameoflife libgol.so $(DEPS)

gameoflife: libgol.so gameoflife.o
	$(CC) $(CFLAGS) -L. -o gameoflife gameoflife.o -lgol

gameoflife.o: gameoflife.c $(DEPS)
	$(CC) -c -fPIC $(CFLAGS)  gameoflife.c

gol.o: gol.c $(DEPS)
	$(CC) -c -fPIC $(CFLAGS)  gol.c

libgol.so : gol.o
	$(CC) gol.o $(CFLAGS) -shared -o libgol.so

basicmake: gameoflife.c gol.c gol.h
	$(CC) $(CFLAGS)