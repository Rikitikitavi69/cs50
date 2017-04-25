/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main()
{
    FILE* file = fopen("card.raw", "r");
    
    if (file == NULL)
    {
        printf("Couldn't open file.\n");
        return 1;
    }
    
    uint8_t jpg[3] = {0xff, 0xd8, 0xff};
    uint8_t jpg1 = 0xe0;
    uint8_t jpg2 = 0xe1;
    
    uint8_t buffer[512];
    
    FILE* output = NULL;
    int file_name_counter = 0;
    
    while (fread(&buffer, sizeof(buffer), 1, file))
    {
        if (buffer[0] == jpg[0] && buffer[1] == jpg[1] && buffer[2] == jpg[2] && (buffer[3] == jpg1 || buffer[3] == jpg2))
        {
            if (output != NULL)
                fclose(output);
            
            char filename[8];
            sprintf(filename, "%03d.jpg", file_name_counter);
            output = fopen(filename, "w");
            file_name_counter++;
        }
        
        if (output != NULL)
            fwrite(&buffer, sizeof(buffer), 1, output);
    } 
    
    if (output != NULL)
        fclose(output);
    
    if (file !=NULL)
        fclose(file);
}

