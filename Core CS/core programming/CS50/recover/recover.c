/** Recover jpeg from card.raw **/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover card.raw\n");
        return 1;
    }

    // open card.raw
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // flag equaling to 0 means the specific header of jpeg has not been found yet
    int flag = 0;

    // count the number of the photos
    int count = 0;

    // remember the photo's name, starting from 000.jpg
    char file_name[8];

    // preset the outfile pointer
    FILE *outptr = NULL;
    while (1)
    {
        // 512B buffer
        BYTE *buffer = malloc(512 * sizeof(BYTE));

        // read 512B block from input file
        int n = fread(buffer, 1, 512, inptr);

        // if fread results less than 512, it means the end of the file
        if (n != 512)
        {
            free(buffer);
            break;
        }

        // recognize the header of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (outptr != NULL)
            {
                fclose(outptr);
            }

            // put the number into the photo name
            sprintf(file_name, "%03i.jpg", count);

            outptr = fopen(file_name,"w");
            if (outptr == NULL)
            {
                fprintf(stderr, "Could not create %s.\n", file_name);
            }

            flag = 1;

            // keep counting the number of photos
            count++;
        }

        // flag equaling to 1 means start to recover
        if (flag == 1)
        {
            fwrite(buffer, 512, 1, outptr);

            // free the memory
            free(buffer);
        }
    }

    return 0;
}