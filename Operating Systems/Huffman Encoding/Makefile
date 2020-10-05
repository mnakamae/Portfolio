CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = hencode hdecode
OBJS = hencode.o hdecode.o functions.o

all : $(OBJS)
	$(CC) $(CFLAGS) -o hencode hencode.o functions.o
	$(CC) $(CFLAGS) -o hdecode hdecode.o functions.o

hencode.o : hencode.c huffman.h
	$(CC) $(CFLAGS) -c hencode.c

hdecode.o : hdecode.c huffman.h
	$(CC) $(CFLAGS) -c hdecode.c

functions.o : functions.c huffman.h
	$(CC) $(CFLAGS) -c functions.c

clean :
	rm *.o $(MAIN) core
