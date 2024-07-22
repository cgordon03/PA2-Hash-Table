//July 9, 2024
//Operating Systems - John Aedo

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50

typedef struct hash_struct
{
  uint32_t hash; //32-bit unsigned integer for the hash value produced by running the name text through the Jenkins one-at-a-time functionLinks to an external site.
  char name[50]; //Arbitrary string up to 50 characters long
  uint32_t salary; //	32-bit unsigned integer (who wants a negative salary, eh?) to represent an annual salary.
  struct hash_struct *next; //pointer to the next node in the list
} hashRecord;

typedef struct {
    char action[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    uint32_t number;
} command_var;

extern FILE *outputFile; //output text file
extern hashRecord *hashTable; //hash record table head
extern command_var *commandArray; //input commands array
extern pthread_rwlock_t rwlock; //the lock


/*extern int numLocks = 0;
extern int numReleases = 0;*/

long long current_timestamp(void);

// Jenkins One-at-a-Time hash function
uint32_t jenkins_one_at_a_time_hash(const char *key);

void parse_line(char *line, command_var *record);
void* funcChooser(void* vargp);

void insert(command_var commandLine);
void del(command_var commandLine);
void print();
uint32_t search(command_var commandLine);

#endif
