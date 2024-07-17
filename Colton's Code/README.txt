Concurrent Hash Table Implementation
====================================

This project implements a concurrent hash table using Jenkins' one-at-a-time hash function and reader-writer locks for synchronization.

Files:
------
- chash.c: Main program file
- Makefile: Build instructions
- hash_table.h: Header file for hash table structure and functions
- hash_table.c: Implementation of hash table functions
- rw_lock.h: Header file for reader-writer lock structure and functions
- rw_lock.c: Implementation of reader-writer lock functions
- commands.txt: Input command file
- output.txt: Output file
- README.txt: Documentation file

Compilation:
------------
Run `make` to compile the project. This will create an executable named `chash`.

Usage:
------
Run `./chash` to execute the program. The program reads commands from `commands.txt` and outputs results to `output.txt`.

Author:
-------
Colton Curtis

Acknowledgments:
----------------
The OSTEP authors for the sample implementations of reader-writer locks.
