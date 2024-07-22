Written by Amoy Marshalleck, Harper Archambault, Clayton Gordon, and Colton Curtis with the use of AI

# PA2-Hash-Table

Concurrent Hash Table Implementation

This project implements a concurrent hash table using pthreads and reader-writer locks.
To build the project, run `make` in the project directory.
The program reads commands from `commands.txt` and produces an output in `output.txt`.

Files:
- chash.c: Main program / locking implementation
- hashTable.h/c: Hash table implementation
- Makefile: Build configuration

Ensure that `commands.txt` is in the same directory as the executable.

We used ChatGPT to generate a function that would take the information from the input file. The information would then put into a struct
that would organize the information into a struct. We took this function and adapted it into our code to match the parameters that were used. Below is the link to our conversation. 

https://chatgpt.com/share/79f3c409-c743-4b44-809e-856d60ff128d
