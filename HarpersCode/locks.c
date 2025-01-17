#include <pthread.h>
#include <stdio.h>
#include "locks.h"
#include "sys/time.h"

//Acquires read lock
void read_lock(pthread_rwlock_t lock, FILE *output){
    pthread_rwlock_rdlock(&lock);
    long long time = current_timestamp();
    fprintf(output, "%lld: READ LOCK ACQUIRED\n", time);
}
//Releases read lock
void read_unlock(pthread_rwlock_t lock, FILE *output){
    pthread_rwlock_unlock(&lock);
    long long time = current_timestamp();
    fprintf(output, "%lld: READ LOCK RELEASED\n", time);
}

//Acquires write lock
void write_lock(pthread_rwlock_t lock, FILE *output){
    pthread_rwlock_wrlock(&lock);
    long long time = current_timestamp();
    fprintf(output, "%lld: WRITE LOCK ACQUIRED\n", time);
}

//Releases write lock
void write_unlock(pthread_rwlock_t lock, FILE *output){
    pthread_rwlock_unlock(&lock); 
    long long time = current_timestamp();
    fprintf(output, "%lld: WRITE LOCK RELEASED\n", time);
}

//Returns current timestamp
long long current_timestamp() {
  struct timeval te;
  gettimeofday(&te, NULL); // get current time
  long long microseconds = (te.tv_sec * 1000000) + te.tv_usec; // calculate milliseconds
  return microseconds;
}