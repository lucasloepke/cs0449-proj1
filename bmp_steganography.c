#include <stdio.h>
#include <string.h>

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
    unsigned short bitCount;        // Number of bits per pixel
    unsigned int compression;       // Compression scheme used
    unsigned int imageSize;         // Image size in bytes
    int xPixelsPerMeter;            // Horizontal resolution
    int yPixelsPerMeter;            // Vertical resolution
    unsigned int colorsUsed;        // Number of colors in the palette
    unsigned int colorsImportant;   // Number of important colors
} DIBHeader;

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} Pixel;
#pragma pack(4)

void info(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("ERROR: File not found.\n");
        return;
    }

    BMPHeader bmpHeader;
    DIBHeader dibHeader;

    // Read BMP header
    if (fread(&bmpHeader, sizeof(BMPHeader), 1, file) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }

    // Check for BMP file
    if (bmpHeader.type != 0x4D42) { // BM in ASCII, reversed for little endian
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }

    // Read DIB header
    if (fread(&dibHeader, sizeof(DIBHeader), 1, file) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }

    // Check DIB header size and RGB bits per pixel
    if (dibHeader.headerSize != 40 || dibHeader.bitCount != 24) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }

    printf("=== BMP Header ===\n");
    printf("Type: BM\n");
    printf("Size: %u\n", bmpHeader.size);
    printf("Reserved 1: %hu\n", bmpHeader.reserved1);
    printf("Reserved 2: %hu\n", bmpHeader.reserved2);
    printf("Image offset: %u\n\n", bmpHeader.offset);

    printf("=== DIB Header ===\n");
    printf("Size: %u\n", dibHeader.headerSize);
    printf("Width: %d\n", dibHeader.width);
    printf("Height: %d\n", dibHeader.height);
    printf("# color planes: %hu\n", dibHeader.planes);
    printf("# bits per pixel: %hu\n", dibHeader.bitCount);
    printf("Compression scheme: %u\n", dibHeader.compression);
    printf("Image size: %u\n", dibHeader.imageSize);
    printf("Horizontal resolution: %d\n", dibHeader.xPixelsPerMeter);
    printf("Vertical resolution: %d\n", dibHeader.yPixelsPerMeter);
    printf("# colors in palette: %u\n", dibHeader.colorsUsed);
    printf("# important colors: %u\n", dibHeader.colorsImportant);

    fclose(file);
}

void reveal(const char *filename) {

}

void hide(const char *filename1, const char *filename2) {

}

int main(int argc, char *argv[]) {
    // printf("Number of arguments: %d\n", argc);
    // for (int i = 0; i < argc; i++) {
    //     printf("argc: %d\n", argc);
    //     printf("Argument %d: %s\n", i, argv[i]);
    // }

    if (argc < 3) {
        printf("ERROR: Missing Arguments.\n");
        return 1;
    }

    if (strcmp(argv[1], "--info") == 0) {
        info(argv[2]);
    } else if (strcmp(argv[1], "--reveal") == 0) {
        reveal(argv[2]);
    } else if (strcmp(argv[1], "--hide") == 0) {
        if (argc < 4) {
            printf("ERROR: Missing Arguments.\n");
            return 1;
        }
        hide(argv[2], argv[3]);
    } else {
        printf("ERROR: Invalid Arguments.\n");
        return 1;
    }

    return 0;
}