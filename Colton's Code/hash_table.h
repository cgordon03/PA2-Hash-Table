#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include <stddef.h>
#include "rw_lock.h"

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

typedef struct {
    hashRecord **table;
    size_t size;
    rw_lock_t *locks;
} hashTable;

uint32_t jenkins_one_at_a_time_hash(const char *key);
void insert(hashTable *table, const char *name, uint32_t salary);
void delete(hashTable *table, const char *name);
hashRecord* search(hashTable *table, const char *name);
void print_table(hashTable *table);

#endif // HASH_TABLE_H
