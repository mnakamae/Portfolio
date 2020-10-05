CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = fw
OBJS = fw.o functions.o
all : $(MAIN)

$(MAIN) : $(OBJS) fw.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

fw.o : fw.c fw.h
	$(CC) $(CFLAGS) -c fw.c

functions.o : functions.c fw.h
	$(CC) $(CFLAGS) -c functions.c

clean :
	rm *.o $(MAIN) core
