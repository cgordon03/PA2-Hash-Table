#ifndef LOCKS_H
#define LOCKS_H

void read_lock(pthread_rwlock_t lock, FILE *output);
void read_unlock(pthread_rwlock_t lock, FILE *output);
void write_lock(pthread_rwlock_t lock, FILE *output);
void write_unlock(pthread_rwlock_t lock, FILE *output);
long long current_timestamp();

#endif