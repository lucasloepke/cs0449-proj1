all: bmp_steganography

bmp_steganography: bmp_steganography.c
	gcc -O0 -g -Wall -Werror -o bmp_steganography bmp_steganography.c

run: bmp_steganography
	./bmp_steganography example.bmp

clean:
	rm -f bmp_steganography
	rm -r bmp_steganography.dSYM
