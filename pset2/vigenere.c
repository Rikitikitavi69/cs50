// check50 2015.fall.pset2.vigenere vigenere.c
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int keynumber(char s)
{
    if (isupper(s))
    {
        return s-65;
    }
    else
    {
        return s-97;
    }
}

int main(int argc, string argv[])
{
    if (argc != 2) 
    {
        printf("You must use exactly 1 command argument!\n");
        return 1; 
    }
    
    for (int i=0, n=strlen(argv[1]); i<n; i++)
    {
        if (isalpha(argv[1][i]) == false)
        {
            printf("A command argument must contain letters only!");
            return 1; 
        }
    }

    string plaintext = GetString();
    int keylength = strlen(argv[1]);
    
    int counter = 0;
    
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                int key = keynumber(argv[1][counter]);
                int letter = ((plaintext[i] + key - 65) % 26) + 65;
                printf("%c", letter); 
                counter++;
            }
            
            if (islower(plaintext[i]))
            {
                int key = keynumber(argv[1][counter]);
                int letter = ((plaintext[i] + key - 97) % 26) + 97;
                printf("%c", letter);
                counter++;
            }
            
            if (counter == keylength)
            {
                counter = 0;
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

