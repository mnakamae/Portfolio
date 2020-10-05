#include "parseline.h"

/*
O = NOT IMPLEMENTED
X = IMPLEMENTED BUT NOT TESTED
- = WORKING

X command too long
X pipeline too deep
X cmd: too many arguments
X invalid null command
X cmd: bad input redirection
X cmd: bad output redirection
X cmd: ambiguous input
O cmd: ambiguous output
*/

struct command ** parseline(int argc, char *argv[], int *num_pipes)
{
   struct command **cmd_arr = (struct command **)calloc(10, sizeof(struct command *));
   struct command *cur;
   char cmd[1028];
   char *line;
   int stage = 0;
   int in_flag = 0, out_flag = 0;
   *num_pipes = 0;

   printf("8-P ");
   if(scanf("%[^\n]%*c", cmd) == EOF) /* HANDLE OVERFLOW */
      return NULL;

   if(strlen(cmd) == 0) /* EMPTY LINE */
   {
      perror("usage: parseline [ -c ] [ -d ] set1 [ set2 ]");
      exit(-1);
   }
   if(strlen(cmd) > 512)
   {
      perror("command too long");
      exit(-1);
   }

   line = strtok(cmd, " "); /* COMMAND */
   cur = create_command(line, stage);
   while(1){
      /* parse here */
      line = strtok(NULL, " "); /* GET NEXT INPUT */
      if(line == NULL)
      {
         cmd_arr[stage] = cur; 
         break;
      }

      if(strcmp(line, "|") == 0){
         (*num_pipes)++;
      }

      if(strcmp(line, "<") == 0) /* INPUT */
      {
         if(stage > 0) /* changing stdin in a child process */
         {
            perror("cmd: ambiguous input");
            exit(-1);
         }

         if(in_flag == 0) /* VALID INPUT */
         {
            strcat(cur->name, " < ");
            line = strtok(NULL, " ");

            strcat(cur->name, line);
            cur->input = open(line, O_RDONLY);
            cur->change = 1;
            in_flag = 1;
         }
         else if(in_flag == 1 || line == NULL)
         {
            perror("cmd: bad input redirection");
            exit(-1);
         }
         
      }
      else if(strcmp(line, ">") == 0) /* OUTPUT */
      {
         if(out_flag == 0) /* VALID OUTPUT */
         {
            strcat(cur->name, " > ");
            line = strtok(NULL, " ");

            strcat(cur->name, line);
            cur->output = open(line, O_RDWR | O_TRUNC | O_CREAT | 0666);
            printf("%d\n", cur->output);
            cur->change = 1;
            out_flag = 1;
         }
         else if(out_flag == 1 || line == NULL)
         {
            perror("cmd: bad output redirection");
            exit(-1);
         }
      }
      else if(strcmp(line, "|") == 0) /* PIPE */
      {
         if(out_flag == 1)
         {
            perror("cmd: ambiguous output");
            exit(-1);
         }
         line = strtok(NULL, " ");
         if(strcmp(line, "|") == 0)
         {
            perror("invalid null command");
            exit(-1);
         }

         if((stage + 1) > 9)
         {
            perror("pipeline too deep");
            exit(-1);
         }

         cmd_arr[stage] = cur; 
         stage++;
         cur = create_command(line, stage);
      }
      else if(line != NULL) /* ARGV */ 
      {
         /*
         if((cur->argc) == 9)
         {
            perror("cmd: too many arguments");
            exit(-1);
         }*/
         strcat(cur->name, " ");
         strcat(cur->name, line);
         strcpy(cur->args[cur->argc], line);
         cur->argc++;
      }
   }
   
   /*for(i = 0; i < 10; i++)
   {
      if(cmd_arr[i] != NULL)
      {
         print_cmd(cmd_arr[i]);
      }
   }
   free(cmd_arr);*/
   return cmd_arr;
}
