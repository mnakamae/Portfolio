#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h>

#include <signal.h>

struct command{
   char name[128];
   int input;
   int output;
   char args[64][64];
   int argc;
   int stage;
   int change;
};

struct command * create_command(char *, int);
char ** format_args(char [64][64], int);
void print_cmd(struct command *);
void free_cmds(struct command **);
struct command ** parseline(int, char *[], int*);
