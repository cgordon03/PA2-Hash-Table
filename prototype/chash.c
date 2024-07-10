#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h> // Include for sleep
#include "hash_table.h"
#include "logging.h"

#define COMMAND_FILE "commands.txt"
#define OUTPUT_FILE "output.txt"

void *execute_command(void *arg);

typedef struct {
    char command[10];
    char name[50];
    uint32_t salary;
} Command;

pthread_rwlock_t rwlock;
HashTable *hash_table;
FILE *output_file;

int main() {
    FILE *command_file = fopen(COMMAND_FILE, "r");
    if (!command_file) {
        perror("Failed to open command file");
        return 1;
    }

    output_file = fopen(OUTPUT_FILE, "w");
    if (!output_file) {
        perror("Failed to open output file");
        return 1;
    }

    int num_commands;
    fscanf(command_file, "threads,%d,0\n", &num_commands);

    pthread_t threads[num_commands];
    Command commands[num_commands];

    hash_table = create_hash_table();
    pthread_rwlock_init(&rwlock, NULL);

    for (int i = 0; i < num_commands; i++) {
        fscanf(command_file, "%[^,],%[^,],%d\n", commands[i].command, commands[i].name, &commands[i].salary);
        pthread_create(&threads[i], NULL, execute_command, &commands[i]);
        usleep(1000); // Add a small delay to ensure different timestamps
    }

    for (int i = 0; i < num_commands; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);
    fclose(command_file);
    fclose(output_file);
    return 0;
}

void *execute_command(void *arg) {
    Command *command = (Command *)arg;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long milliseconds_since_epoch = (long long)(tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

    if (strcmp(command->command, "insert") == 0) {
        pthread_rwlock_wrlock(&rwlock);
        log_lock("WRITE LOCK ACQUIRED", milliseconds_since_epoch);
        insert(hash_table, command->name, command->salary);
        log_operation("INSERT", command->name, command->salary);
        log_lock("WRITE LOCK RELEASED", milliseconds_since_epoch);
        pthread_rwlock_unlock(&rwlock);
    } else if (strcmp(command->command, "delete") == 0) {
        pthread_rwlock_wrlock(&rwlock);
        log_lock("WRITE LOCK ACQUIRED", milliseconds_since_epoch);
        delete(hash_table, command->name);
        log_operation("DELETE", command->name, 0);
        log_lock("WRITE LOCK RELEASED", milliseconds_since_epoch);
        pthread_rwlock_unlock(&rwlock);
    } else if (strcmp(command->command, "search") == 0) {
        pthread_rwlock_rdlock(&rwlock);
        log_lock("READ LOCK ACQUIRED", milliseconds_since_epoch);
        uint32_t salary = search(hash_table, command->name);
        log_operation("SEARCH", command->name, salary);
        if (salary != 0) {
            log_search_result(command->name, salary);
        } else {
            fprintf(output_file, "No Record Found\n");
        }
        log_lock("READ LOCK RELEASED", milliseconds_since_epoch);
        pthread_rwlock_unlock(&rwlock);
    } else if (strcmp(command->command, "print") == 0) {
        pthread_rwlock_rdlock(&rwlock);
        log_lock("READ LOCK ACQUIRED", milliseconds_since_epoch);
        print_table(hash_table, output_file); // Pass output_file as argument
        log_lock("READ LOCK RELEASED", milliseconds_since_epoch);
        pthread_rwlock_unlock(&rwlock);
    }
    return NULL;
}
