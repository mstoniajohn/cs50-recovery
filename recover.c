#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    //open memory card
    FILE *file = fopen(argv[1], "r");
    //if error occurs send message
    if (file == NULL)
    {
        printf("Error occured opening the file\n");
        return 1;
    }
    //set buffer to 512 bytes to reag jpegs

    typedef uint8_t BYTE;
    BYTE buffer[512];
    //initials number of bytes read from stream
    size_t bytes_read;
    //bool for jpegs
    bool first_jpeg = false;
    FILE *current_file;
    char current_filename[100];
    int current_filenumber = 0;
    bool found_jpeg = false;
    //read files into buffer from raw file
    while (true)
    {
        bytes_read = fread(buffer, sizeof(BYTE), 512, file);
        if (bytes_read == 0)
        {
            //break because the end of file
            break;
        }
        //if it is the start of jpeg
        //check if first 3 buffers are 0xff 0xd8 0xff
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            found_jpeg = true;
            //check if first jpeg else make first jpeg
            if (!first_jpeg)
            {
                first_jpeg = true;
            }
            //if not 1st close file and open new file
            else
            {
                fclose(current_file);

            }
            sprintf(current_filename, "%03i.jpg", current_filenumber);
            current_file = fopen(current_filename, "w");
            fwrite(buffer, sizeof(BYTE), bytes_read, current_file);
            current_filenumber++;

        }
        else
        {
            if (found_jpeg)
            {
                fwrite(buffer, sizeof(BYTE), bytes_read, current_file);
            }

        }
    }
    fclose(file);
    fclose(current_file);
    return 0;

    //the first 3 bytes of a jpeg file is 0xff 0xd8 0xff
    //last byte is 0xe0 up t0 0xef




}