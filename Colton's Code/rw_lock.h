#ifndef RW_LOCK_H
#define RW_LOCK_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t readers_proceed;
    pthread_cond_t writer_proceed;
    int waiting_writers;
    int active_readers;
    int active_writers;
} rw_lock_t;

void rw_lock_init(rw_lock_t *lock);
void rw_lock_destroy(rw_lock_t *lock);
void rw_read_lock(rw_lock_t *lock);
void rw_read_unlock(rw_lock_t *lock);
void rw_write_lock(rw_lock_t *lock);
void rw_write_unlock(rw_lock_t *lock);

#endif // RW_LOCK_H
