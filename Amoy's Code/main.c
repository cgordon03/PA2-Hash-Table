//Amoy Marshalleck
//July 9, 2024
//Operating Systems - John Aedo

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "main.h"

int main(int argc, char* argv []) {
    //for executing the code
    //gcc main.c -Wno-unused-result -lm
    //./a.out commands.txt
    
    FILE* inputFile;
    inputFile = fopen(argv[1], "r");

    //Checks if the input file is valid.
    if (inputFile == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    //when command is "thread"
    // The number of commands to be processed in the file. 
    //This allows you to setup a dynamic array of pthread_t structs. 
    //This is always the first line of the command file.  
    //Each command after this runs in a separate thread, so if you have 12 commands, this line should read threads 12,0
    char *command;
    int numThreads = 0;
    char fileChar;
    //fscanf(inputFile, "%m[^,]", &command);
    //printf("%s", command); //for checking
    // if(strcmp(command, "threads") == 0){
    //     fscanf(inputFile, "%d", &numThreads);
    //     printf("%d", numThreads);
    // }

    fscanf(inputFile, "%d", &numThreads);
    printf("%d", numThreads); //to check
    
    fclose(inputFile);
    return 0;
}