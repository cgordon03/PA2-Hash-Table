#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50

typedef struct {
    char action[MAX_NAME_LENGTH];
    char name[MAX_NAME_LENGTH];
    int number;
} Record;

//ChatGPT generated
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

//insert(key, values);
//delete(key);
//search(key);

int main() {
    FILE *file = fopen("sample.txt", "r");
    if (file == NULL) {
        perror("Unable to open file!");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    Record records[100];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        parse_line(line, &records[count]);
        count++;
    }

    fclose(file);

    // Print the records
    for (int i = 0; i < count; i++) {
        printf("Record %d:\n", i + 1);
        printf("Action: %s\n", records[i].action);
        printf("Name: %s\n", records[i].name);
        printf("Number: %d\n\n", records[i].number);
    }

    return 0;
}
