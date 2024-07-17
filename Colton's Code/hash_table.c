#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

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

void insert(hashTable *table, const char *name, uint32_t salary) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    size_t index = hash % table->size;
    
    rw_write_lock(&table->locks[index]);
    
    hashRecord *node = table->table[index];
    while (node) {
        if (node->hash == hash && strcmp(node->name, name) == 0) {
            node->salary = salary;
            rw_write_unlock(&table->locks[index]);
            return;
        }
        node = node->next;
    }
    
    node = malloc(sizeof(hashRecord));
    node->hash = hash;
    strcpy(node->name, name);
    node->salary = salary;
    node->next = table->table[index];
    table->table[index] = node;
    
    rw_write_unlock(&table->locks[index]);
}

void delete(hashTable *table, const char *name) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    size_t index = hash % table->size;
    
    rw_write_lock(&table->locks[index]);
    
    hashRecord *node = table->table[index];
    hashRecord *prev = NULL;
    while (node) {
        if (node->hash == hash && strcmp(node->name, name) == 0) {
            if (prev) {
                prev->next = node->next;
            } else {
                table->table[index] = node->next;
            }
            free(node);
            rw_write_unlock(&table->locks[index]);
            return;
        }
        prev = node;
        node = node->next;
    }
    
    rw_write_unlock(&table->locks[index]);
}

hashRecord* search(hashTable *table, const char *name) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    size_t index = hash % table->size;
    
    rw_read_lock(&table->locks[index]);
    
    hashRecord *node = table->table[index];
    while (node) {
        if (node->hash == hash && strcmp(node->name, name) == 0) {
            rw_read_unlock(&table->locks[index]);
            return node;
        }
        node = node->next;
    }
    
    rw_read_unlock(&table->locks[index]);
    return NULL;
}

void print_table(hashTable *table) {
    for (size_t i = 0; i < table->size; i++) {
        rw_read_lock(&table->locks[i]);
        hashRecord *node = table->table[i];
        while (node) {
            printf("%u,%s,%u\n", node->hash, node->name, node->salary);
            node = node->next;
        }
        rw_read_unlock(&table->locks[i]);
    }
}
