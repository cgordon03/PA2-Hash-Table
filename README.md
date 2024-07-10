# PA2-Hash-Table

Concurrent Hash Table Implementation

This project implements a concurrent hash table using pthreads and reader-writer locks.
To build the project, run `make` in the project directory.
The program reads commands from `commands.txt` and produces an output in `output.txt`.

Files:
- chash.c: Main program
- hash_table.h/c: Hash table implementation
- logging.h/c: Logging functions
- Makefile: Build configuration

Ensure that `commands.txt` is in the same directory as the executable.
