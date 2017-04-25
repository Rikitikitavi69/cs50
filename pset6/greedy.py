from cs50 import *

def main():

    while True:
        print("O hai! How much change is owed?\n")
        change = get_float() * 100
        if change >= 0:
            break
    
    coins = [25, 10, 5, 1]
    count = 0
    
    for coin in coins:
        while change >= coin:
            count += 1
            change -= coin
            
    print(count)
            
if __name__ == "__main__":
    main()