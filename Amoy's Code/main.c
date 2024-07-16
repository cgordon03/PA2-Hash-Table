//Amoy Marshalleck
//July 9, 2024
//Operating Systems - John Aedo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include "main.h"

char** getLineValues(char* line){
    
    char** res = (char**)malloc(sizeof(char*)*3);
    char* token = (char*)malloc(sizeof(char*));
    
    token = strtok(line, ",");
    int count = 0;
    while(token != NULL){

        res[count] = token;
        //printf("%s\n", token); //for checking
        count++;
        token = strtok(NULL, ",");
    }

    return res;
}

void* commandChooser(FILE *inputFile, void* vargp){

    char *line = (char*)malloc(sizeof(char*));
    fscanf(inputFile, "%s", line); //the line is not copied correctly.
    printf("%s\n", line); //to check
    //char** lineArray = getLineValues(line);

    //printf("%s %s %s\n", lineArray[0], lineArray[1], lineArray[2]); //to check
    //this is the part that isn't working
    // if(strcmp(lineArray[0], "insert") == 0){
 
    // }else if(strcmp(lineArray[0], "delete") == 0){
        
    // }else if(strcmp(lineArray[0], "search") == 0){
        
    // }else if(strcmp(lineArray[0], "print") == 0){
        
    // }else{

    //     printf("Invalid command\n");
    //     //exit(0);
    //     //return NULL;
    // } 

    return NULL;
}

int main(){

    FILE *inputFile = fopen("commands.txt", "r");
    FILE *outputFile = fopen("output.txt", "w");

    //cehcks if file is valid
    if(inputFile == NULL)
        printf("Error opening file\n");

    //Step1: taking in the number of threads
    char *line = (char*)malloc(sizeof(char*));
    char *line2 = (char*)malloc(sizeof(char*));
    fscanf(inputFile, "%[^\n]", line);
    //printf("%s\n", line); //to check
    
    char **lineArray = getLineValues(line); //stores an array of strings. Each string is a section of the 1st line.

    int numThreads = atoi(lineArray[1]);
    //printf("%d\n", numThreads); //to check
    
    fprintf(outputFile, "Running %d threads\n", numThreads);
    
    //Step 2: Making an array of threads
    pthread_t *threadsArray = (pthread_t*)malloc(sizeof(pthread_t)*numThreads);

    //Step 3: Going through each thread.
    for(int i = 0; i < numThreads; i++){

        //pthread_create(&threadsArray[i], NULL, (void*)commandChooser, inputFile);
    }
    
    fclose(inputFile);
    fclose(outputFile);
    return 0;
}

//for executing the code
// gcc main.c -Wno-unused-result -lm
//./a.out commands.txt
