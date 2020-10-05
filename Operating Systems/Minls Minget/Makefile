CC      	= gcc
CFLAGS 	= -Wall -g -fpic
#CCFLAGS 	= -g -std=c89 -pedantic -Wall -Werror -D NDEBUG
#LDFLAGS 	= -lm 
SOURCES  = parse.c filesys.c
OBJECTS  = parse.o filesys.o
LSSOURCES = minls.c
LSOBJECTS = minls.o
GETSOURCES = minget.c
GETOBJECTS = minget.o


all: minls minget
.PHONY :all

minls: $(LSOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o minls $(LSOBJECTS) $(OBJECTS)

minget: $(GETOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) -o minget $(GETOBJECTS) $(OBJECTS)

minget.o: minget.c filesys.c
	$(CC) $(CFLAGS) -c -o minget.o minget.c

minls.o: minls.c filesys.c
	$(CC) $(CFLAGS) -c -o minls.o minls.c

parser.o: parse.c
	$(CC) $(CFLAGS) -c -o parse.o parse.c

filesys.o: filesys.c
	$(CC) $(CFLAGS) -c -o filesys.o filesys.c
	

clean:
	rm -f $(OBJECTS) $(LSOBJECTS) $(GETOBJECTS) *- TAGS minls minget