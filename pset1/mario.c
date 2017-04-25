#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height;
    int spaces;
    int hashes;
    
    do 
    {
        printf("height: ");
        height = GetInt();
    }
    while (height < 0 || height > 23);
    
    for (int i = 1; i <= height; i++)
        {
            for (spaces = height-i; spaces != 0; spaces--)
            {
                printf(" ");
            }
            for (hashes = 0; hashes <= i; hashes++)
            {
                printf("#");
            }
            printf("\n");
        }
    
    return 0;
}