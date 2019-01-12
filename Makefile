CC=gcc
CFLAGS=-O3 -Wall -Wextra -ansi -pedantic

main_image: main_image.c image.o evolve_image.o
	$(CC) $(CFLAGS) -o main_image main_image.c evolve_image.o evolve_pixel.o image.o

main_row: main_row.c image.o evolve_row.o
	$(CC) $(CFLAGS) -o main_row main_row.c evolve_row.o evolve_pixel.o image.o

evolve_image.o: evolve_image.c evolve_image.h evolve_pixel.o
	$(CC) $(CFLAGS) -c -o evolve_image.o evolve_image.c

evolve_row.o: evolve_row.c evolve_row.h evolve_pixel.o
	$(CC) $(CFLAGS) -c -o evolve_row.o evolve_row.c

evolve_pixel.o: evolve_pixel.c evolve_pixel.h
	$(CC) $(CFLAGS) -c -o evolve_pixel.o evolve_pixel.c

image.o: image.c image.h
	$(CC) $(CFLAGS) -c -o image.o image.c

clean:
	rm -rf main_image main_row *.o *.dSYM *.png *.ppm *.gif *.mp4

mp4: main_image
	@printf 'Started building MP4 in memory.\n'
	@./main_image | ffmpeg -y -r 60 -f image2pipe -i - -vcodec libx264 -pix_fmt yuv420p video.mp4 2> /dev/null
	@printf '\33[2K\rDone building MP4.\n'
