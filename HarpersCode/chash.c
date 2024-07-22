#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "hash_table.h"
#include "locks.h"
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
    pthread_rwlock_t lock = PTHREAD_RWLOCK_INITIALIZER;

    //Gets the number of threads from the first line of the file
    if (fgets(line, sizeof(line), file)){
        recordNum = get_thread_num(line);
    }
    fprintf(output,"Running %d threads\n", recordNum);

    //Reads all of the commands from the file and stores in array
    Record records[recordNum];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        parse_line(line, &records[count]);
        count++;
    }

    fclose(file);

    int lock_acq = 0, lock_rel = 0;
    int peopleCount = 0;

    for (int i = 0; i < count; i++) {
 
        //Executes insert operation
        if(strcmp("insert", records[i].action) == 0){
            write_lock(lock, output);
            lock_acq++;

            uint32_t hash = jenkins_one_at_a_time_hash(records[i].name);
            long long time = current_timestamp();
            fprintf(output,"%lld: INSERT, %u, %s, %d\n", time, hash, records[i].name, records[i].number);
            insert(&hash_table, records[i].name, records[i].number);
            peopleCount++;

            write_unlock(lock, output);
            lock_rel++;
        }

        //Executes print operation
        else if (strcmp("print", records[i].action) == 0){
            read_lock(lock, output);
            lock_acq++;
            print(&hash_table, output);
            read_unlock(lock, output);
            lock_rel++;
        }

        //Executes search operation
        else if (strcmp("search", records[i].action) == 0){
            read_lock(lock, output);
            lock_acq++;

            fprintf(output,"SEARCH, %s\n", records[i].name);
            int searchNum = in_table(&hash_table, records[i].name);
            if(searchNum){
                struct hash_struct searchNode = search(&hash_table, records[i].name);
                u_int32_t hash = jenkins_one_at_a_time_hash(searchNode.name);
                long long time = current_timestamp();
                fprintf(output,"%lld: %u, %s, %u\n", time, hash, searchNode.name, searchNode.salary);
            }
            else{
                long long time = current_timestamp();
                fprintf(output, "%lld: NOT FOUND\n", time);
            }

            read_unlock(lock, output);
            lock_rel++;
        }

        //Executes delete operation
        else if (strcmp("delete", records[i].action) == 0){
            write_lock(lock, output);
            lock_acq++;

            long long time = current_timestamp();
            fprintf(output, "%lld: DELETE AWAKENED\n", time);
            delete(&hash_table, records[i].name);
            time = current_timestamp();
            fprintf(output,"%lld: DELETE, %s\n", time, records[i].name);
            peopleCount--;

            write_unlock(lock, output);
            lock_rel++;
        }
    }

    fprintf(output, "Finished all threads\n");
    fprintf(output, "Number of lock acquisitions: %d\n", lock_acq);
    fprintf(output, "Number of lock releases: %d\n", lock_rel);

    read_lock(lock, output);
    sorted_print (&hash_table, output, peopleCount);
    read_unlock(lock, output);

    fclose(output);
    return 0;
}

//Gets the number of theads from the input file
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


