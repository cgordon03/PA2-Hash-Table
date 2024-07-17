#include "rw_lock.h"

void rw_lock_init(rw_lock_t *lock) {
    pthread_mutex_init(&lock->mutex, NULL);
    pthread_cond_init(&lock->readers_proceed, NULL);
    pthread_cond_init(&lock->writer_proceed, NULL);
    lock->waiting_writers = 0;
    lock->active_readers = 0;
    lock->active_writers = 0;
}

void rw_lock_destroy(rw_lock_t *lock) {
    pthread_mutex_destroy(&lock->mutex);
    pthread_cond_destroy(&lock->readers_proceed);
    pthread_cond_destroy(&lock->writer_proceed);
}

void rw_read_lock(rw_lock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    while (lock->active_writers || lock->waiting_writers > 0) {
        pthread_cond_wait(&lock->readers_proceed, &lock->mutex);
    }
    lock->active_readers++;
    pthread_mutex_unlock(&lock->mutex);
}

void rw_read_unlock(rw_lock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->active_readers--;
    if (lock->active_readers == 0 && lock->waiting_writers > 0) {
        pthread_cond_signal(&lock->writer_proceed);
    }
    pthread_mutex_unlock(&lock->mutex);
}

void rw_write_lock(rw_lock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->waiting_writers++;
    while (lock->active_writers || lock->active_readers > 0) {
        pthread_cond_wait(&lock->writer_proceed, &lock->mutex);
    }
    lock->waiting_writers--;
    lock->active_writers++;
    pthread_mutex_unlock(&lock->mutex);
}

void rw_write_unlock(rw_lock_t *lock) {
    pthread_mutex_lock(&lock->mutex);
    lock->active_writers--;
    if (lock->waiting_writers > 0) {
        pthread_cond_signal(&lock->writer_proceed);
    } else {
        pthread_cond_broadcast(&lock->readers_proceed);
    }
    pthread_mutex_unlock(&lock->mutex);
}
