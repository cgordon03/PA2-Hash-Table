CC = gcc
CFLAGS = -pthread -Wall -Wextra -O2
TARGET = chash

all: $(TARGET)

$(TARGET): chash.o hash_table.o rw_lock.o
	$(CC) $(CFLAGS) -o $(TARGET) chash.o hash_table.o rw_lock.o

chash.o: chash.c hash_table.h rw_lock.h
	$(CC) $(CFLAGS) -c chash.c

hash_table.o: hash_table.c hash_table.h rw_lock.h
	$(CC) $(CFLAGS) -c hash_table.c

rw_lock.o: rw_lock.c rw_lock.h
	$(CC) $(CFLAGS) -c rw_lock.c

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
