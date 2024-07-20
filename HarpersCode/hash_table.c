#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash_table.h"

struct htable *make_table() {
    struct htable *ht = (struct htable *) malloc(sizeof (struct htable));
    //ht->entries = NULL;
    return ht;
}

uint32_t jenkins_one_at_a_time_hash(const char *key){
    size_t i = 0;
    uint32_t hash = 0;
    while (i != strlen(key)) {
        hash += key[i++];
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

//Assists insert function
struct hash_struct* insert_front(struct hash_struct *front, const char *name, uint32_t salary, uint32_t hash) {

    struct hash_struct *temp;

    // Create temp node and link it to front.
    temp = (struct hash_struct*)malloc(sizeof(struct hash_struct));
    strcpy(temp->name, name);
    temp->hash = hash;
    temp->salary = salary;
    temp->next = front;

    // Return the new front of the list.
    return temp;
}

void insert(struct htable *h, const char *key, uint32_t salary){

    uint32_t hash = jenkins_one_at_a_time_hash(key);
    uint32_t index = hash  % 1024;
    h->entries[index] = insert_front(h->entries[index], key, salary, index);
}

struct hash_struct* delNode(struct hash_struct *front, const char *name){
    struct hash_struct *node;
    struct hash_struct *del;

    node = front;

    if (node != NULL){
        if(strcmp(node->name, name) == 0){
            del = node->next;
            free(node);
            return del;
        }

        while(node->next != NULL){
            if(strcmp(node->next->name,name) == 0){
                del = node->next;
                node->next = node->next->next;
                free(del);
                return front;
            }
            node = node->next;
        }
    }
    return node;
}

void delete(struct htable *h, const char *key){
    uint32_t hash = jenkins_one_at_a_time_hash(key);
    uint32_t index = hash  % 1024;

    h->entries[index] = delNode(h->entries[index], key);
}

struct hash_struct search (struct htable *h, const char *name){
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    uint32_t index = hash  % 1024;

    struct hash_struct *node = h->entries[index];

    while (node != NULL){
        if (strcmp(node->name, name) == 0){
            return *node;
        }
         node = node->next;
    }

    return *node; 
}

void print (struct htable *table, FILE *output){
    for(int i=0; i<1024; i++){
        if (table->entries[i] != NULL){
            uint32_t hash = jenkins_one_at_a_time_hash(table->entries[i]->name);
            fprintf(output, "%u,%s,%u\n", hash, table->entries[i]->name, table->entries[i]->salary);
        }
    }
}

