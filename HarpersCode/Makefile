CC = gcc
CFLAGS = -pthread -Wall -Wextra -O2
TARGET = chash

all: $(TARGET)

$(TARGET): chash.o hash_table.o
	$(CC) $(CFLAGS) -o $(TARGET) chash.o hash_table.o

chash.o: chash.c hash_table.h
	$(CC) $(CFLAGS) -c chash.c

hash_table.o: hash_table.c hash_table.h 
	$(CC) $(CFLAGS) -c hash_table.c

clean:
	rm -f *.o $(TARGET)
