#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    int header_size = sizeof(WAVHEADER);

    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: reverse.exe input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    FILE *fpin = fopen(argv[1], "r");
    if (fpin == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    // Read header
    WAVHEADER in_header;
    fread(&in_header, 1, header_size, fpin);

    // Use check_format to ensure WAV format
    if (!check_format(in_header))
    {
        printf("Could not verify input is wav file\n");
        return 1;
    }

    // Open output file for writing
    FILE *fpout = fopen(argv[2], "a");
    if (fpout == NULL)
    {
        printf("Could not create output file pointer\n");
        return 1;
    }

    // Write header to file
    fwrite(&in_header, 1, header_size, fpout);

    // Use get_block_size to calculate size of block
    // TODO #7

    // Write reversed audio to file
    // TODO #8
    fclose(fpin);
    fclose(fpout);
}

int check_format(WAVHEADER header)
{
    if (header.format[0] == 'W' && header.format[1] == 'A' && header.format[2] == 'V' && header.format[3] == 'E')
    {
        return 1;
    }

    return 0;
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    return 0;
}