CC=gcc
CFLAGS=-Weverything -ansi -pedantic

markov_color: markov_color.c image.o
	$(CC) $(CFLAGS) -o markov_color markov_color.c image.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c -o image.o image.c

clean:
	rm -f markov_color image.o *.ppm *.png
