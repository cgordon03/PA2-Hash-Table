Written by Amoy Marshalleck with the use of AI

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
