// check50 2015.fall.pset2.caesar caesar.c
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc != 2) 
    {
        printf("You must use exactly 1 command argument!\n");
        return 1; 
    }
    
    int k = atoi(argv[1]);
    string plaintext = GetString();

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                int letter = ((plaintext[i] + k - 65) % 26) + 65;
                printf("%c", letter); 
            }
            
            if (islower(plaintext[i]))
            {
                int letter = ((plaintext[i] + k - 97) % 26) + 97;
                printf("%c", letter);
            }
        
        }
        
        else
        {
        printf("%c", plaintext[i]);
        }
        
    }
    
    printf("\n");
    return 0;
}