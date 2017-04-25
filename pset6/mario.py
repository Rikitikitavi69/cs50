import cs50

def main():
    
    while True:
        print("Height:", end="")
        height = cs50.get_int()
        if height > 0 and height < 24:
            break
    
    spaces = height - 1
    hashes = 2
        
    for i in range(height):
        print(" " * spaces + "#" * hashes + "\n")
        spaces -= 1
        hashes += 1
    
if __name__ == "__main__":
    main()