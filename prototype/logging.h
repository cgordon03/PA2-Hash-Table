#ifndef LOGGING_H
#define LOGGING_H

void log_lock(const char *message, long long timestamp);
void log_operation(const char *operation, const char *name, uint32_t salary);
void log_search_result(const char *name, uint32_t salary);

#endif // LOGGING_H
