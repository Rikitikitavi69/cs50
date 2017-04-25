// check50 2015.fall.pset2.initials initials.c
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    char * name = GetString();
    
    printf("%c", toupper(name[0]));
    
    for (int i=0, j=strlen(name); i < j; i++)
        if(name[i] == ' ')
            printf("%c", toupper(name[i+1]));
        
    printf("\n");
}