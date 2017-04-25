#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float quarter_25 = 25;
    float dime_10 = 10;
    float nickel_5 = 5;
    float penny_1 = 1;
    float change;
    int coins = 0;
    
    do {
        printf("O hai! How much change is owed?\n");
        change = GetFloat();
    } while (change<=0);
    
    change = change * 100;
    change = round(change);
    
    while(change >= quarter_25) {
        change -= quarter_25;
        coins++;
    } 
    
    while(change >= dime_10) {
        change -= dime_10;
        coins++;
    } 
    
    while(change >= nickel_5) {
        change -= nickel_5;
        coins++;
    } 
    
    while(change >= penny_1) {
        change -= penny_1;
        coins++;
    } 
    
    printf("%i", coins);
    printf("\n");
    
    return 0;
}