CC = gcc
CFLAGS = -pthread -Wall -Wextra -O2
TARGET = chash

all: $(TARGET)

$(TARGET): chash.o hashTable.o
	$(CC) $(CFLAGS) -o $(TARGET) chash.o hashTable.o

chash.o: chash.c hashTable.h 
	$(CC) $(CFLAGS) -c chash.c

hash_table.o: hashTable.c hashTable.h
	$(CC) $(CFLAGS) -c hashTable.c

clean:
	rm -f *.o $(TARGET)
