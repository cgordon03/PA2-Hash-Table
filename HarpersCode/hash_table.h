#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct hash_struct{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

struct htable {
    struct hash_struct* entries [1024];
};

struct htable *make_table();
uint32_t jenkins_one_at_a_time_hash(const char *key);
void insert(struct htable *h, const char *key, uint32_t salary);
void delete(struct htable *h, const char *key);
int in_table (struct htable *h, const char *name);
struct hash_struct search (struct htable *h, const char *name);
void print(struct htable *table, FILE *output);
void sorted_print (struct htable *table, FILE *output, int peopleCount);
int compareHash(const void *a, const void *b);
void sortHashRecords(hashRecord *records, size_t numRecords);

#endif