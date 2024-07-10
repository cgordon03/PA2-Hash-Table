#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

typedef struct hash_struct
{
  uint32_t hash; //32-bit unsigned integer for the hash value produced by running the name text through the Jenkins one-at-a-time functionLinks to an external site.
  char name[50]; //Arbitrary string up to 50 characters long
  uint32_t salary; //	32-bit unsigned integer (who wants a negative salary, eh?) to represent an annual salary.
  struct hash_struct *next; //pointer to the next node in the list
} hashRecord;
