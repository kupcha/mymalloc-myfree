CC = gcc
CCFLAGS = -Wall

memgrind: memgrind.o mymalloc.o
	$(CC) $(CCFLAGS) memgrind.o mymalloc.o -o memgrind

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CCFLAGS) -c mymalloc.c

memgrind.o: memgrind.c mymalloc.h
	$(CC) $(CCFLAGS) -c memgrind.c

clean:
	rm -f memgrind *.o
