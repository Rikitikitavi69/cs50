from cs50 import *
import sys

def main():
    
    if len(sys.argv) != 2:
        print("You must use exactly 1 command argument!\n")
        return
    
    key = int(sys.argv[1])
    
    plain_text = get_string()
    
    for char in plain_text:
        
        if char.isalpha():
            if char.isupper():
                print(chr(((ord(char) + key - 65) % 26) + 65), end="")
            if char.islower():
                print(chr(((ord(char) + key - 97) % 26) + 97), end="")
        else:
            print(char, end="")
    
    print("\n")
    
if __name__ == "__main__":
    main()