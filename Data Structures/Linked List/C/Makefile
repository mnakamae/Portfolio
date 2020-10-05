CC = gcc
CFLAGS = -Wall -pedantic
MAIN = LinkedList
OBJS = LinkedList.o functions.o
all : $(MAIN)

$(MAIN) : $(OBJS) LinkedList.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

LinkedList.o : LinkedList.c LinkedList.h
	$(CC) $(CFLAGS) -c LinkedList.c

functions.o : functions.c LinkedList.h
	$(CC) $(CFLAGS) -c functions.c

clean :
	rm *.o $(MAIN) core
