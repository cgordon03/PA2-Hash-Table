#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "logging.h"
#include "hash_table.h" // Include the header file for jenkins_one_at_a_time_hash

extern FILE *output_file;

void log_lock(const char *message, long long timestamp) {
    fprintf(output_file, "%lld,%s\n", timestamp, message);
}

void log_operation(const char *operation, const char *name, uint32_t salary) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long milliseconds_since_epoch = (long long)(tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

    if (strcmp(operation, "INSERT") == 0 || strcmp(operation, "DELETE") == 0 || strcmp(operation, "SEARCH") == 0 || strcmp(operation, "PRINT") == 0) {
        fprintf(output_file, "%lld,%s,%s,%u\n", milliseconds_since_epoch, operation, name, salary);
    }
}

void log_search_result(const char *name, uint32_t salary) {
    uint32_t hash = jenkins_one_at_a_time_hash(name);
    fprintf(output_file, "%u,%s,%u\n", hash, name, salary);
}
