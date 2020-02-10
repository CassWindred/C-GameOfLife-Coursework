CC=gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -lm
DEPS = gol.h

.PHONY : all
all : gameoflife libgol.so $(DEPS)

gameoflife: libgol.so gameoflife.o
	$(CC) $(CFLAGS) -L. -Wl,-rpath=\$$ORIGIN -o gameoflife gameoflife.o -lgol

gameoflife.o: gameoflife.c $(DEPS)
	$(CC) -c -fPIC $(CFLAGS)  gameoflife.c

gol.o: gol.c $(DEPS)
	$(CC) -c -fPIC $(CFLAGS)  gol.c

libgol.so : gol.o
	$(CC) gol.o $(CFLAGS) -shared -o libgol.so

.phony: clean
clean:
	rm -f gameoflife gameoflife.o gol.o libgol.so