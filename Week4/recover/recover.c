#include <stdio.h>
#include <stdlib.h>

typedef __uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover.exe file\n");
        return 1;
    }

    BYTE bytes_buffer[512];
    int size_buffer = sizeof(bytes_buffer);
    int size_item = sizeof(BYTE);
    int file_count = 0;
    char file_name[9]; // holds up to name 999.jpeg\0

    FILE *fpin = fopen(argv[1], "r");
    if (fpin == NULL)
    {
        printf("Cannot open file\n");
        return 1;
    }

    FILE *fpout = NULL;

    while (fread(bytes_buffer, size_item, size_buffer, fpin) == size_buffer)
    {
        if (bytes_buffer[0] == 0xff && bytes_buffer[1] == 0xd8 && bytes_buffer[2] == 0xff)
        {
            if (file_count > 0)
            {
                fclose(fpout);
            }

            sprintf(file_name, "%03i.jpeg", file_count);
            file_count++;

            fpout = fopen(file_name, "a");
            if (fpout == NULL)
            {
                return 3;
            }

            fwrite(bytes_buffer, size_item, size_buffer, fpout);

        }else if (fpout != NULL)
        {
            fwrite(bytes_buffer, size_item, size_buffer, fpout);
        }
    }
    if (fpout != NULL)
    {
        fclose(fpout);
    }
    fclose(fpin);
}
