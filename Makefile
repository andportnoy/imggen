CC=gcc
CFLAGS=-O2 -Wall -Wextra -ansi -pedantic

image_by_image: image_by_image.c image.o evolve.o
	$(CC) $(CFLAGS) -o image_by_image image_by_image.c image.o evolve.o

row_by_row: row_by_row.c image.o evolve.o
	$(CC) $(CFLAGS) -o row_by_row row_by_row.c image.o evolve.o

image.o: image.c image.h
	$(CC) $(CFLAGS) -c -o image.o image.c

evolve.o: evolve.c evolve.h
	$(CC) $(CFLAGS) -c -o evolve.o evolve.c

clean:
	rm -rf image_by_image row_by_row *.o *.dSYM *.png *.ppm *.gif

gif: png
	@printf 'Building GIF...'
	@convert -delay 10 -loop 0 $$(ls -1 images/*.png | sort -V) animation.gif
	@rm images/*.png
	@rmdir images
	@printf '\33[2K\rDone building GIF.\n'

png: ppm
	@printf 'Converting .ppm to .png...'
	@mogrify -format png images/*.ppm
	@rm images/*.ppm
	@printf '\33[2K\rDone converting.\n'

ppm: image_by_image
	@mkdir -p images
	@./image_by_image
