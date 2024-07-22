//Amoy Marshalleck
//July 9, 2024
//Operating Systems - John Aedo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include "sys/time.h"
#include "hashTable.h"

long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL); // get current time
  long long microseconds = (te.tv_sec * 1000000) + te.tv_usec; // calculate milliseconds
  return microseconds;
}

// Jenkins One-at-a-Time hash function
uint32_t jenkins_one_at_a_time_hash(const char *key) {
    uint32_t hash = 0;
    while (*key) {
        hash += *key++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

//takes the command line
//if the person is new, inserts the values into the hash table.
//if the person is old, updates their values.
//works
void insert(command_var commandLine){

    uint32_t newHash = jenkins_one_at_a_time_hash(commandLine.name);
    
    //intializing new Node
    hashRecord *newNode = (hashRecord *)malloc(sizeof(hashRecord));
    newNode->hash = newHash;
    strcpy(newNode->name, commandLine.name);
    newNode->salary = commandLine.number;
    newNode->next = NULL;
    
    //if the hash table is empty, insert the new node.
    //if the first hash value is greater than the new hash value, insert the new node at the head.
    if(hashTable == NULL || hashTable->hash > newHash){
    
        newNode->next = hashTable;
        hashTable = newNode;
    }else{

        hashRecord *current = hashTable;
        //if the hash value is less than the current hash value, insert the new node in between the current node and the
        while(current->next != NULL && current->next->hash < newNode->hash){
        
            current = current->next;
        }
        
        if(current->next != NULL && current->next->hash == newNode->hash){
        
            current->next->salary = newNode->salary;
        }else{
        
            newNode->next = current->next;
            current->next = newNode;
        }
    }
}

//takes the command line and hashTable
//deletes the command line from the hasTable
//there is aproblem with corretly deleting the person.
//works
void del(command_var commandLine){
    
    //computes the hash value of the key
    uint32_t newHash = jenkins_one_at_a_time_hash(commandLine.name);
    
    hashRecord *prev = NULL;
    hashRecord *current = hashTable;
    
    //search through the hash table for person
    while(current != NULL){
        
        if(current->hash == newHash){
        
          if(prev != NULL){
            
            prev->next = current->next;
          }else{
              
            hashTable = current->next;
          }
            
            free(current);
            return;
        }
        
        prev = current;
        current = current->next;
    }
    
    //when name is not found, return with no changes.
}

//takes the command line
//returns the hash value, if the entry is found.
//returns NULL, if the entry isn't found.
uint32_t search(command_var commandLine){

    //printf("was used\n"); //to check if used.
  //computes the hash value of the key
  uint32_t newHash = jenkins_one_at_a_time_hash(commandLine.name);

    hashRecord *current = hashTable;
    //printf("was used\n"); //to check if used.
    
  //search through the hash table for person
  while(current != NULL){
      
      if(current->hash == newHash){

        //final output , and releasing
        //printf("search\n");//to check
        return newHash;
      }

      current = current->next;
  }

  //-1 is a stand in for null, because this function cannot return a negative number.
  return -1;
}

//takes the hashTable
//prints all contents of the hasTable
void print(){
    
    //fprintf(outputFile, "is running\n");
    hashRecord *current = hashTable;
    while (current != NULL) {

        fprintf(outputFile, "%lld: %u,%s,%d\n", current_timestamp(), current->hash, current->name, current->salary);
        //printf("%d,%s,%d\n", current->hash, current->name, current->salary);
        current = current->next;
    }
}