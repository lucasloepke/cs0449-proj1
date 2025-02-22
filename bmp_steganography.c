#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    FILE *file = fopen(filename, "rb"); // read binary
    if (file == NULL) {
        printf("ERROR: File not found.\n");
        return;
    }

    BMPHeader bmpHeader;
    DIBHeader dibHeader;

    // read BMP header
    if (fread(&bmpHeader, sizeof(BMPHeader), 1, file) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }
    // verify BMP file
    if (bmpHeader.type != 0x4D42) { // ASCII BM reversed for little endian
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }

    // read DIB header
    if (fread(&dibHeader, sizeof(DIBHeader), 1, file) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }
    // verify DIB header size and RGB bits per pixel
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
    FILE *file = fopen(filename, "rb+"); // read and write binary
    if (file == NULL) {
        printf("ERROR: File not found.\n");
        return;
    }

    BMPHeader bmpHeader;
    DIBHeader dibHeader;

    // read BMP header
    if (fread(&bmpHeader, sizeof(BMPHeader), 1, file) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }
    // verify BMP file
    if (bmpHeader.type != 0x4D42) { // ASCII BM reversed for little endian
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }

    // read DIB header
    if (fread(&dibHeader, sizeof(DIBHeader), 1, file) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }
    // verify DIB header size and RGB bits per pixel
    if (dibHeader.headerSize != 40 || dibHeader.bitCount != 24) {
        printf("ERROR: The format is not supported.\n");
        fclose(file);
        return;
    }

    // move to start of pixel data
    fseek(file, bmpHeader.offset, SEEK_SET);

    // calculate padding bytes per row as a multiple of 4
    int padding = (4 - ((dibHeader.width * sizeof(Pixel)) % 4)) % 4;

    for (int y = 0; y < abs(dibHeader.height); y++) {   // row
        for (int x = 0; x < dibHeader.width; x++) {     // pixel
            Pixel pixel;
            
            // read pixel
            if (fread(&pixel, sizeof(Pixel), 1, file) != 1) {
                printf("ERROR: The format is not supported.\n");
                fclose(file);
                return;
            }

            // move back to same pixel position
            fseek(file, -sizeof(Pixel), SEEK_CUR);

            // swap 4 MSB with 4 LSB for each color component
            pixel.blue = (pixel.blue << 4) | (pixel.blue >> 4);
            pixel.green = (pixel.green << 4) | (pixel.green >> 4);
            pixel.red = (pixel.red << 4) | (pixel.red >> 4);

            // write back modified pixels
            if (fwrite(&pixel, sizeof(Pixel), 1, file) != 1) {
                printf("ERROR: The format is not supported.\n");
                fclose(file);
                return;
            }
        }

        // skip padding bytes at the end of row
        if (padding > 0) fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
}

void hide(const char *filename1, const char *filename2) {
    FILE *file1 = fopen(filename1, "rb+");  // read and write binary
    FILE *file2 = fopen(filename2, "rb");   // read binary
    if (file1 == NULL || file2 == NULL) {
        printf("ERROR: File not found.\n");
        if (file1) fclose(file1);
        if (file2) fclose(file2);
        return;
    }

    BMPHeader bmpHeader1, bmpHeader2;
    DIBHeader dibHeader1, dibHeader2;

    // read BMP1 header
    if (fread(&bmpHeader1, sizeof(BMPHeader), 1, file1) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }
    // verify BMP1 file
    if (bmpHeader1.type != 0x4D42) { // ASCII BM reversed for little endian
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }

    // read DIB1 header
    if (fread(&dibHeader1, sizeof(DIBHeader), 1, file1) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }
    // verify DIB1 header size and RGB bits per pixel
    if (dibHeader1.headerSize != 40 || dibHeader1.bitCount != 24) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }

    // read BMP2 header
    if (fread(&bmpHeader2, sizeof(BMPHeader), 1, file2) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }
    // verify BMP2 file
    if (bmpHeader2.type != 0x4D42) { // ASCII BM reversed for little endian
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }

    // read DIB2 header
    if (fread(&dibHeader2, sizeof(DIBHeader), 1, file2) != 1) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }
    // verify DIB2 header size and RGB bits per pixel
    if (dibHeader2.headerSize != 40 || dibHeader2.bitCount != 24) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }

    // ensure dimensions match
    if (dibHeader1.width != dibHeader2.width || dibHeader1.height != dibHeader2.height) {
        printf("ERROR: The format is not supported.\n");
        fclose(file1);
        fclose(file2);
        return;
    }

    // move to start of pixel data
    fseek(file1, bmpHeader1.offset, SEEK_SET);
    fseek(file2, bmpHeader2.offset, SEEK_SET);

    // calculate padding bytes per row as a multiple of 4
    int padding = (4 - ((dibHeader1.width * sizeof(Pixel)) % 4)) % 4;

    for (int y = 0; y < abs(dibHeader1.height); y++) {   // row
        for (int x = 0; x < dibHeader1.width; x++) {     // pixel
            Pixel pixel1, pixel2;
            
            // read pixels from both files
            if (fread(&pixel1, sizeof(Pixel), 1, file1) != 1 || 
                fread(&pixel2, sizeof(Pixel), 1, file2) != 1) {
                printf("ERROR: The format is not supported.\n");
                fclose(file1);
                fclose(file2);
                return;
            }

            // move back to write position in file1
            fseek(file1, -sizeof(Pixel), SEEK_CUR);

            // merge pixels: keep 4 MSB of pixel1, set LSB to 4 MSB of pixel2
            pixel1.blue = (pixel1.blue & 0xF0) | ((pixel2.blue & 0xF0) >> 4);
            pixel1.green = (pixel1.green & 0xF0) | ((pixel2.green & 0xF0) >> 4);
            pixel1.red = (pixel1.red & 0xF0) | ((pixel2.red & 0xF0) >> 4);

            // write merged pixel back to file1
            if (fwrite(&pixel1, sizeof(Pixel), 1, file1) != 1) {
                printf("ERROR: The format is not supported.\n");
                fclose(file1);
                fclose(file2);
                return;
            }
        }

        // skip padding bytes at the end of row in both files
        if (padding > 0) {
            fseek(file1, padding, SEEK_CUR);
            fseek(file2, padding, SEEK_CUR);
        }
    }
    
    fclose(file1);
    fclose(file2);
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