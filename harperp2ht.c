#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50
#define TABLE_SIZE 60000

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

struct htable {
    struct hash_struct* entries[TABLE_SIZE];
};

typedef struct {
    char action[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    uint32_t number;
} Record;

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

//Gets the number of threads
int  get_thread_num(char *line) {
    
    int threadNum;

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

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

//Inserts to the front of the linked list
struct hash_struct* insert_front(struct hash_struct *front, char word[MAX_NAME_LENGTH], uint32_t salary, uint32_t hash) {

    struct hash_struct *temp;

    // Create temp node and link it to front.
    temp = (struct hash_struct*)malloc(sizeof(struct hash_struct));
    strcpy(temp->name, word);
    temp->hash = hash;
    temp->salary = salary;
    temp->next = front;

    // Return the new front of the list.
    return temp;
}

//Inserts into hash table
void insert(struct htable *h, char key[MAX_NAME_LENGTH], uint32_t salary){
    uint32_t hash;

    uint32_t uint_key =atoi(key);

    //hash = jenkins_one_at_a_time_hash(uint_key, TABLE_SIZE);
    //h->entries[hash] = insert_front(h->entries[hash], key, salary, hash);
}

void delete(int key);

void search(int key);

int main() {
    FILE *file = fopen("sample.txt", "r");
    if (file == NULL) {
        perror("Unable to open file!");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;
    int recordNum =0;
    struct htable hash_table;

    //Gets the number of threads from the first line of the file
    if (fgets(line, sizeof(line), file)){
        recordNum = get_thread_num(line);
    }

    //Initialize table
    for(int i=0; i<TABLE_SIZE; i++){
        hash_table.entries[i] = NULL;
    }
    
    //Allocates memory for array of records
    printf("%d", recordNum);
    Record *records = malloc(recordNum * sizeof(Record));

    //Reads all of the commands from the file
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        parse_line(line, &records[count]);
        count++;
    }

    fclose(file);

    // Print the records
    for (int i = 0; i < recordNum; i++) {
        printf("Record %d:\n", i + 1);
        printf("Action: %s\n", records[i].action);
        printf("Name: %s\n", records[i].name);
        printf("Number: %d\n\n", records[i].number);
    }

    for (int i = 0; i < count; i++) {
 
        if (strcmp("insert", records[i].action) == 0){
            insert(&hash_table, records[i].name, records[i].number);
            printf("do insert\n");
        }
        else if (strcmp("print", records[i].action) == 0){
            printf("do print\n");
        }
        else if (strcmp("search", records[i].action) == 0){
            printf("do search\n");
        }
        else if (strcmp("delete", records[i].action) == 0){
            printf("do delete\n");
        }
    }

    return 0;
}