CC = gcc
CFLAGS = -Wall -pedantic -g
MAIN = mush
OBJS = mush.o parseline.o functions.o
all : $(MAIN)

$(MAIN) : $(OBJS) parseline.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

mush.o : mush.c parseline.h
	$(CC) $(CFLAGS) -c mush.c

parseline.o : parseline.c parseline.h
	$(CC) $(CFLAGS) -c parseline.c

functions.o : functions.c parseline.h
	$(CC) $(CFLAGS) -c functions.c

clean :
	rm *.o $(MAIN) core
