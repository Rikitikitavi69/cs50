/**
 * generate.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constant
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Checks if user started the programm with one or two command line arguments
    if (argc != 2 && argc != 3)
    {
        printf("Usage: generate n [s]\n");
        return 1;
    }

    // Converts 1st command line argument n (which is how many pseudorandom numbers to generate) from string to integer.
    int n = atoi(argv[1]);

    // Checks if the user entered 2nd command line argument and sets it as a seed for drand48 function. Otherwise, sets it to time(NULL), since this value will always change.
    if (argc == 3)
    {
        srand48((long int) atoi(argv[2]));
    }
    else
    {
        srand48((long int) time(NULL));
    }

    // Calls drand48 function which returns random value over the interval 0.0 - 1.0, multiple it by LIMIT value (default is 65536) and prints the result n times.
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
