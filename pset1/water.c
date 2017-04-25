#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("minutes: ");
    int min = GetInt();
    int bottles = 12 * min;
    printf("bottles: %i\n", bottles);
}