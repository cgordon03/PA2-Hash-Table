#ifndef LOCKS_H
#define LOCKS_H

void read_lock(pthread_rwlock_t lock, FILE *output, int lock_acq);
void read_unlock(pthread_rwlock_t lock, FILE *output, int lock_rel);
void write_lock(pthread_rwlock_t lock, FILE *output, int lock_acq);
void write_unlock(pthread_rwlock_t lock, FILE *output, int lock_rel);
long long current_timestamp();

#endif