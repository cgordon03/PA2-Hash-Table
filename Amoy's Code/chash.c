//Amoy Marshalleck
//July 9, 2024
//Operating Systems - John Aedo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include "hashTable.c"

//ChatGPT generated
//Separates the information from input file
void parse_line(char *line, command_var *record) {

    char *token = strtok(line, ",");
    if (token != NULL) {
        strcpy(record->action, token);
        token = strtok(NULL, ",");
    }
    if (token != NULL) {
        strcpy(record->name, token);
        token = strtok(NULL, ",");
    }
    if (token != NULL) {
        record->number = atoi(token);
    }
}

void* funcChooser(void* vargp){

  command_var *commandLine = (command_var*)vargp;
  
  if(strcmp(commandLine->action, "insert") == 0){

      //Initial command logging
      fprintf(outputFile, "%lld: INSERT,%u,%s,%d\n", current_timestamp(), jenkins_one_at_a_time_hash(commandLine->name), commandLine->name, commandLine->number);
      pthread_rwlock_wrlock(&rwlock);
      fprintf(outputFile, "%lld: WRITE LOCK ACQUIRED\n", current_timestamp());
      numLocks++;
      
      insert(*commandLine);

      //Final command logging
      fprintf(outputFile, "%lld: WRITE LOCK RELEASED\n", current_timestamp());
      pthread_rwlock_unlock(&rwlock);
      numReleases++;
  }else if(strcmp(commandLine->action, "delete") == 0){

      //intialing printing to output file and locking
      fprintf(outputFile, "%lld: DELETE AWAKENED\n", current_timestamp());
      fprintf(outputFile, "%lld: DELETE %u,%s\n", current_timestamp(), jenkins_one_at_a_time_hash(commandLine->name), commandLine->name);
      pthread_rwlock_wrlock(&rwlock);
      fprintf(outputFile, "%lld: WRITE LOCK ACQUIRED\n", current_timestamp());
      numLocks++;
      
      del(*commandLine);

      //final printing to the output file and unlocking
      fprintf(outputFile, "%lld: WRITE LOCK RELEASED\n", current_timestamp());
      pthread_rwlock_unlock(&rwlock);
      numReleases++;
  }else if(strcmp(commandLine->action, "print") == 0){

      //intialing printing to output file and locking
      pthread_rwlock_rdlock(&rwlock);
      fprintf(outputFile, "%lld: READ LOCK ACQUIRED\n", current_timestamp());
      numLocks++;
      
        print();

      //final printing to the output file and unlocking
      fprintf(outputFile, "%lld: WRITE LOCK RELEASED\n", current_timestamp());
      pthread_rwlock_unlock(&rwlock);
      numReleases++;
  }else if(strcmp(commandLine->action, "search") == 0){

      //intial printing to output file and locking.
      pthread_rwlock_rdlock(&rwlock);
      fprintf(outputFile, "%lld: READ LOCK ACQUIRED\n", current_timestamp());
      numLocks++;
      
      if(search(*commandLine) > 0){

          fprintf(outputFile, "%lld: SEARCH,%u,%s\n", current_timestamp(), jenkins_one_at_a_time_hash(commandLine->name), commandLine->name);
      }else{

          fprintf(outputFile, "%lld: SEARCH: NOT Found\n", current_timestamp());
      }

        //final output , and releasing
      fprintf(outputFile, "%lld: READ LOCK RELEASED\n", current_timestamp());
      pthread_rwlock_unlock(&rwlock);
      numReleases++;
  }else{

      fprintf(outputFile, "This command is not valid. Please try again.\n");
      exit(0);
  }

  return NULL;
}

int main(void) {

  FILE *inputFile = fopen("commands.txt", "r");
  outputFile = fopen("output.txt", "w");

  //cehcks if file is valid
  if(inputFile == NULL)
      printf("Error opening file\n");

  //Step1: taking in the number of threads
  char line[MAX_LINE_LENGTH];
  int count = 0;
  command_var threadCommand;

  if(fgets(line, sizeof(line), inputFile)){
      line[strcspn(line, "\n")] = '\0'; // Remove newline character
      parse_line(line, &threadCommand);
  }
  //printf("%s %s %d\n", record.action, record.name, record.number); //to check

  int numThreads = atoi(threadCommand.name);
  fprintf(outputFile, "Running %d threads\n", numThreads); //to check

  //Steps 2: Make and initialize an array of commands of size numThreads.
  commandArray = (command_var *)malloc(numThreads * sizeof(command_var));

  while(fgets(line, sizeof(line), inputFile)){
      line[strcspn(line, "\n")] = '\0'; // Remove newline character
      parse_line(line, &commandArray[count]);
      //printf("%s %s %d\n", commandArray[count].action, commandArray[count].name, commandArray[count].number); //to check    

      count++;
  }
  
  //Step 3: Close input File because no longer needed.
  fclose(inputFile);

  //Step 4: Initializes the hashTable
  hashTable = (hashRecord *)malloc(sizeof(hashRecord));
  hashTable->next = NULL;

  //Step 5: creating threads
  pthread_t *threads = (pthread_t *)malloc(numThreads * sizeof(pthread_t));

  //Step 6: Execute each command using a thread.
  for(int i = 0; i < numThreads; i++){
    //funcChooser(commandArray[i]);
    pthread_create(&threads[i], NULL, funcChooser, &commandArray[i]);
  }

    //Step 7: Final Print
    fprintf(outputFile, "Finished all threads.\n");
    fprintf(outputFile, "Number of lock acquisitions: %d\n", numLocks);
    fprintf(outputFile, "Number of lock releases: %d\n", numReleases);
    print();
  
  fclose(outputFile);
  return 0;
}

//for executing the code
// gcc chash.c -Wno-unused-result -lm
//./a.out commands.txt