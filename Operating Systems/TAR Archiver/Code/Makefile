CC = gcc
CFLAGS = -Wall -pedantic
MAIN = mytar
OBJS = mytar.o funcs.o
all : $(MAIN)

$(MAIN) : $(OBJS) funcs.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mytar.o : mytar.c funcs.h
	$(CC) $(CFLAGS) -c mytar.c

funcs.o : funcs.c funcs.h
	$(CC) $(CFLAGS) -c funcs.c

clean :
	rm *.o $(MAIN) core
