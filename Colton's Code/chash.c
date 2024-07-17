#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "hash_table.h"
#include "rw_lock.h"

// Command structure
typedef struct {
    char command[10];
    char name[50];
    uint32_t salary;
    hashTable *table;
    FILE *output_file;
} command_t;

// Global counters for lock operations
int lock_acquisitions = 0;
int lock_releases = 0;

// Function prototypes
void execute_command(hashTable *table, const char *command, const char *name, uint32_t salary, FILE *output_file);
void *thread_func(void *arg);
void execute_commands(hashTable *table, const char *filename, FILE *output_file);
void log_message(FILE *output_file, const char *message);
uint64_t get_timestamp();

int main() {
    // Initialize hash table
    size_t table_size = 1024;
    hashTable table;
    table.size = table_size;
    table.table = calloc(table_size, sizeof(hashRecord *));
    table.locks = malloc(table_size * sizeof(rw_lock_t));
    for (size_t i = 0; i < table_size; i++) {
        rw_lock_init(&table.locks[i]);
    }

    // Open output file
    FILE *output_file = fopen("output.txt", "w");
    if (!output_file) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    // Read and execute commands
    execute_commands(&table, "commands.txt", output_file);

    // Clean up
    for (size_t i = 0; i < table_size; i++) {
        rw_lock_destroy(&table.locks[i]);
    }
    free(table.locks);
    free(table.table);

    // Print summary information
    fprintf(output_file, "\nNumber of lock acquisitions: %d\n", lock_acquisitions);
    fprintf(output_file, "Number of lock releases: %d\n", lock_releases);
    fprintf(output_file, "Final Table:\n");

    for (size_t i = 0; i < table_size; i++) {
        rw_read_lock(&table.locks[i]);
        lock_acquisitions++;
        hashRecord *node = table.table[i];
        while (node) {
            fprintf(output_file, "%u,%s,%u\n", node->hash, node->name, node->salary);
            node = node->next;
        }
        rw_read_unlock(&table.locks[i]);
        lock_releases++;
    }

    fclose(output_file);
    return 0;
}

void execute_command(hashTable *table, const char *command, const char *name, uint32_t salary, FILE *output_file) {
    char log_entry[256];
    uint64_t timestamp = get_timestamp();

    if (strcmp(command, "insert") == 0) {
        insert(table, name, salary);
        snprintf(log_entry, sizeof(log_entry), "%llu,INSERT,%s,%u", timestamp, name, salary);
    } else if (strcmp(command, "delete") == 0) {
        delete(table, name);
        snprintf(log_entry, sizeof(log_entry), "%llu,DELETE,%s", timestamp, name);
    } else if (strcmp(command, "search") == 0) {
        hashRecord *record = search(table, name);
        if (record) {
            snprintf(log_entry, sizeof(log_entry), "%u,%s,%u", record->hash, record->name, record->salary);
        } else {
            snprintf(log_entry, sizeof(log_entry), "No Record Found");
        }
    } else if (strcmp(command, "print") == 0) {
        snprintf(log_entry, sizeof(log_entry), "%llu,PRINT", timestamp);
        log_message(output_file, log_entry);

        for (size_t i = 0; i < table->size; i++) {
            rw_read_lock(&table->locks[i]);
            lock_acquisitions++;
            hashRecord *node = table->table[i];
            while (node) {
                fprintf(output_file, "%u,%s,%u\n", node->hash, node->name, node->salary);
                node = node->next;
            }
            rw_read_unlock(&table->locks[i]);
            lock_releases++;
        }
        snprintf(log_entry, sizeof(log_entry), "Final Table Printed");
    }

    log_message(output_file, log_entry);
}

void *thread_func(void *arg) {
    command_t *cmd = (command_t *)arg;
    execute_command(cmd->table, cmd->command, cmd->name, cmd->salary, cmd->output_file);
    return NULL;
}

void execute_commands(hashTable *table, const char *filename, FILE *output_file) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open commands file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int num_threads;
    fgets(line, sizeof(line), file);
    sscanf(line, "threads,%d,0", &num_threads);

    pthread_t threads[num_threads];
    command_t commands[num_threads];
    int command_index = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%u", commands[command_index].command, commands[command_index].name, &commands[command_index].salary);
        commands[command_index].table = table;
        commands[command_index].output_file = output_file;
        pthread_create(&threads[command_index], NULL, thread_func, &commands[command_index]);
        command_index++;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    fclose(file);
}

void log_message(FILE *output_file, const char *message) {
    fprintf(output_file, "%s\n", message);
}

uint64_t get_timestamp() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}
