#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash_table.h"

HashTable *create_hash_table() {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->head = NULL;
    return table;
}

void insert(HashTable *table, const char *name, uint32_t salary) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    hashRecord *prev = NULL, *current = table->head;

    while (current != NULL && current->hash < hash) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->hash == hash) {
        strcpy(current->name, name);
        current->salary = salary;
    } else {
        hashRecord *new_node = (hashRecord *)malloc(sizeof(hashRecord));
        new_node->hash = hash;
        strcpy(new_node->name, name);
        new_node->salary = salary;
        new_node->next = current;

        if (prev == NULL) {
            table->head = new_node;
        } else {
            prev->next = new_node;
        }
    }
}

void delete(HashTable *table, const char *name) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    hashRecord *prev = NULL, *current = table->head;

    while (current != NULL && current->hash < hash) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->hash == hash) {
        if (prev == NULL) {
            table->head = current->next;
        } else {
            prev->next = current->next;
        }
        free(current);
    }
}

uint32_t search(HashTable *table, const char *name) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    hashRecord *current = table->head;

    while (current != NULL && current->hash < hash) {
        current = current->next;
    }

    if (current != NULL && current->hash == hash) {
        return current->salary;
    }

    return 0;
}

void print_table(HashTable *table, FILE *output) {  // Update function definition
    hashRecord *current = table->head;
    while (current != NULL) {
        fprintf(output, "%u,%s,%u\n", current->hash, current->name, current->salary);
        current = current->next;
    }
}

uint32_t jenkins_one_at_a_time_hash(const char *key) {
    uint32_t hash, i;
    size_t len = strlen(key);

    for (hash = i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}
