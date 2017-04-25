/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    
    int min = 0;
    int max = n - 1;
    int midpoint;
    
    while (max >= min)
    {
        midpoint = (min + max) / 2;
       
        if (value == values[midpoint])
        {
            return true;
        }
        
        else if (value < values[midpoint]) 
        { 
            max = midpoint-1; 
            
        }
        
        else
        {
            min = midpoint+1;
        }
    
    } 
    return false;
}


/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int swap_counter;
    
    do 
    {
        swap_counter = 0;
        for (int i = 0; i != n-1; i++)
        {
            int temp;
            if (values[i] > values[i+1])
            {
                temp = values[i];
                values[i] = values[i+1];
                values[i+1] = temp;
                swap_counter++;
            }
        }
        
    } while(swap_counter != 0);
    
    return;
}



/* Linear Search

bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    for (int i = 0; i < n; i++)
    {
        if (value == values[i])
        {
            return true;
        }

    }
    return false;
}

*/ 