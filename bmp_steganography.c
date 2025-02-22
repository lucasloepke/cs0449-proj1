#include <stdio.h>

#pragma pack(1)
typedef struct {
    unsigned short type;            // Format identifier (magic number) [BM]
    unsigned int size;              // Size of the file in bytes
    unsigned short reserved1;       // A two-byte reserved value
    unsigned short reserved2;       // Another two-byte reserved value
    unsigned int offset;            // Offset to the start of the pixel array
} BMPHeader;

typedef struct {
    unsigned int headerSize;        // Size of this DIB header in bytes
    int width;                      // Width of the image in pixels
    int height;                     // Height of the image in pixels
    unsigned short planes;          // Number of color planes (don’t worry)
    unsigned short bitCount;        // 	Number of bits per pixel
    unsigned int compression;       // Compression scheme used
    unsigned int imageSize;         // Image size in bytes
    int xPixelsPerMeter;            // Horizontal resolution
    int yPixelsPerMeter;            // 	Vertical resolution
    unsigned int colorsUsed;        // 	Number of colors in the palette
    unsigned int colorsImportant;   // Number of important colors
} DIBHeader;

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;
#pragma pack(4)

int main(int argc, char *argv[]) {
    printf("Number of arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Argument %d: %s\n", i, argv[i]);
    }
    return 0;
}

void info(const char *filename) {

}

void reveal(const char *filename) {

}

void hide(const char *filename1, const char *filename2) {

}