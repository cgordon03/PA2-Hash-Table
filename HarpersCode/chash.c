#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "hash_table.h"
#include "sys/time.h"

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50

typedef struct {
    char action[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    uint32_t number;
} Record;

long long current_timestamp();

int  get_thread_num(char *line);
void parse_line(char *line, Record *record);

int main() {
    FILE *file = fopen("commands.txt", "r");
    if (file == NULL) {
        perror("Unable to open input file!");
        return 1;
    }

    FILE *output = fopen("output.txt", "w");
    if (output == NULL){
        perror("Unable to open output file");
        return 1;
    }

    char line[256];
    int count = 0;
    int recordNum =0;

    struct htable hash_table = *make_table();
    pthread_rwlock_t lock;

    //Gets the number of threads from the first line of the file
    if (fgets(line, sizeof(line), file)){
        recordNum = get_thread_num(line);
    }
    fprintf(output,"Running %d threads\n", recordNum);

    //Allocates memory for array of records
    printf("%d", recordNum);
    //Record *records = malloc(recordNum * sizeof(Record));
    Record records[recordNum];

    //Reads all of the commands from the file
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        parse_line(line, &records[count]);
        count++;
    }

    fclose(file);

    long long time = 0;

    for (int i = 0; i < count; i++) {
 
        if(strcmp("insert", records[i].action) == 0){
            uint32_t hash = jenkins_one_at_a_time_hash(records[i].name);
            fprintf(output,"INSERT, %u, %s, %d\n", hash, records[i].name, records[i].number);
            pthread_rwlock_wrlock(&lock);
            time = current_timestamp();

            fprintf(output, "%lld, WRITE LOCK ACQUIRED\n", time);
            insert(&hash_table, records[i].name, records[i].number);

            time = current_timestamp();
            fprintf(output, "%lld, WRITE LOCK RELEASED\n", time);
            pthread_rwlock_unlock(&lock);
        }
        else if (strcmp("print", records[i].action) == 0){
            pthread_rwlock_rdlock(&lock);
            time = current_timestamp();
            fprintf(output, "%lld, READ LOCK ACQUIRED\n", time);

            print(&hash_table, output);

            time = current_timestamp();
            fprintf(output, "%lld, READ LOCK RELEASED\n", time);
            pthread_rwlock_unlock(&lock);
        }
        else if (strcmp("search", records[i].action) == 0){
            pthread_rwlock_rdlock(&lock);
            time = current_timestamp();
            fprintf(output, "%lld, READ LOCK ACQUIRED\n", time);

            fprintf(output,"SEARCH, %s\n", records[i].name);
            struct hash_struct searchNode = search(&hash_table, records[i].name);
            if (strcmp(searchNode.name,records[i].name) == 0){
                u_int32_t hash = jenkins_one_at_a_time_hash(searchNode.name);
                fprintf(output,"%u, %s, %u\n", hash, searchNode.name, searchNode.salary);
            }
            else{
                fprintf(output, "Person not found");
            }

            time = current_timestamp();
            fprintf(output, "%lld, READ LOCK RELEASED\n", time);
            pthread_rwlock_unlock(&lock);
        }
        else if (strcmp("delete", records[i].action) == 0){
            pthread_rwlock_wrlock(&lock);
            time = current_timestamp();
            fprintf(output, "%lld, WRITE LOCK ACQUIRED\n", time);

            delete(&hash_table, records[i].name);
            fprintf(output,"DELETE, %s\n", records[i].name);

            time = current_timestamp();
            fprintf(output, "%lld, WRITE LOCK RELEASED\n", time);
            pthread_rwlock_unlock(&lock);
        }
    }

    fclose(output);
    return 0;
}

int get_thread_num(char *line) {
    
    int threadNum = 0;

    char *token = strtok(line, ",");
    if (token != NULL) {
        token = strtok(NULL, ",");
    }
    if (token != NULL) {
        threadNum = atoi(token);
        token = strtok(NULL, ",");
    }
    return threadNum;
}

//ChatGPT generated
//Separates the information from input file
void parse_line(char *line, Record *record) {

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

long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL); // get current time
  long long microseconds = (te.tv_sec * 1000000) + te.tv_usec; // calculate milliseconds
  return microseconds;
}


