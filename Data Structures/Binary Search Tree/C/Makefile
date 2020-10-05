CC = gcc
CFLAGS = -Wall -pedantic
MAIN = BST
OBJS = BST.o functions.o
all : $(MAIN)

$(MAIN) : $(OBJS) BST.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

BST.o : BST.c BST.h
	$(CC) $(CFLAGS) -c BST.c

functions.o : functions.c BST.h
	$(CC) $(CFLAGS) -c functions.c

clean :
	rm *.o $(MAIN) core
