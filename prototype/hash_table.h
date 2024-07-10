#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

typedef struct {
    hashRecord *head;
} HashTable;

HashTable *create_hash_table();
void insert(HashTable *table, const char *name, uint32_t salary);
void delete(HashTable *table, const char *name);
uint32_t search(HashTable *table, const char *name);
void print_table(HashTable *table, FILE *output);  // Update function prototype
uint32_t jenkins_one_at_a_time_hash(const char *key);

#endif // HASH_TABLE_H
