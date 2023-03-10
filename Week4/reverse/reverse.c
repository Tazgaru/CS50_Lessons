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
    int block_size = get_block_size(in_header);

    // Write reversed audio to file
    int data_size = in_header.chunkSize + 8 - header_size; // chunk size is total file size -8 bytes
    int block_count = data_size / block_size;

    BYTE block_buffer[block_size];

    fseek(fpin, -block_size, SEEK_END); // go to start of last block in file

    for (int i = 0; i < block_count; i ++)
    {
        fread(block_buffer, 1, block_size, fpin);
        fseek(fpin, -block_size * 2, SEEK_CUR);
        fwrite(block_buffer, 1, block_size, fpout);
    }

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
    return header.numChannels * header.bitsPerSample / 8;
}