CC=gcc
CFLAGS=-Weverything -ansi -pedantic

markov_color: markov_color.c image.o evolve.o
	$(CC) $(CFLAGS) -o markov_color markov_color.c image.o evolve.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c -o image.o image.c

evolve.o: evolve.c evolve.h
	$(CC) $(CFLAGS) -c -o evolve.o evolve.c

clean:
	rm -f markov_color *.o *.ppm *.png
