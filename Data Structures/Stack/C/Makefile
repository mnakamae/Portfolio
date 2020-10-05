CC = gcc
CFLAGS = -Wall -pedantic
MAIN = Stack
OBJS = Stack.o functions.o
all : $(MAIN)

$(MAIN) : $(OBJS) Stack.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

Stack.o : Stack.c Stack.h
	$(CC) $(CFLAGS) -c Stack.c

functions.o : functions.c Stack.h
	$(CC) $(CFLAGS) -c functions.c

clean :
	rm *.o $(MAIN) core
