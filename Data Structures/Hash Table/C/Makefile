CC = gcc
CFLAGS = -Wall -pedantic
MAIN = Hashtable
OBJS = Hashtable.o functions.o
all : $(MAIN)

$(MAIN) : $(OBJS) Hashtable.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

Hashtable.o : Hashtable.c Hashtable.h
	$(CC) $(CFLAGS) -c Hashtable.c

functions.o : functions.c Hashtable.h
	$(CC) $(CFLAGS) -c functions.c

clean :
	rm *.o $(MAIN) core
