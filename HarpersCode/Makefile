CC = gcc
CFLAGS = -pthread -Wall -Wextra -O2
TARGET = chash

all: $(TARGET)

$(TARGET): chash.o hash_table.o locks.o
	$(CC) $(CFLAGS) -o $(TARGET) chash.o hash_table.o locks.o

chash.o: chash.c hash_table.h locks.h
	$(CC) $(CFLAGS) -c chash.c

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c hash_table.c

locks.o: locks.c locks.h
	$(CC) $(CFLAGS) -c locks.c

clean:
	rm -f *.o $(TARGET)
