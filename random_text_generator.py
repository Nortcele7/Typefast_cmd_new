import random
import string

# Function to generate a random word with symbols and numbers
def generate_word():
    length = random.randint(4, 8)  # Word length between 4 and 8 characters
    word = ''.join(random.choices(string.ascii_letters + string.digits + string.punctuation, k=length))
    return word

# Generate 1000 lines, each with 10 random words
file_path_symbols = "symbols_numbers_text_file.txt"

with open(file_path_symbols, "w") as file:
    for _ in range(1000):
        words = [generate_word() for _ in range(10)]
        file.write(" ".join(words) + "\n")

file_path_symbols
