CC      	= gcc
CFLAGS 	= -Wall -g -fpic
#CCFLAGS 	= -g -std=c89 -pedantic -Wall -Werror -D NDEBUG
#LDFLAGS 	= -lm 
SOURCES  = lwp.c 
OBJECTS  = lwp.o scheduler.o

intel-all: liblwp.so 

liblwp.so: lwp.o scheduler.o magic64.S
	$(CC) $(CFLAGS) -shared -o $@ scheduler.o lwp.o magic64.S 

lwp.o: lwp.c
	$(CC) $(CFLAGS) -c -o lwp.o lwp.c

scheduler.o: scheduler.c
	$(CC) $(CFLAGS) -c -o scheduler.o scheduler.c

clean:
	rm -f $(OBJECTS) *- TAGS 