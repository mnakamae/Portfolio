#include "parseline.h"

struct command * create_command(char *cmd, int stage)
{
   struct command *tmp = (struct command *)calloc(1, sizeof(struct command));
   strcpy(tmp->name, cmd);
   strcpy(tmp->args[0], cmd);
   tmp->output = 0;
   tmp->input =  0;
   tmp->argc = 1;
   tmp->stage = stage;
   tmp->change = 0;
   return tmp;
}

char ** format_args(char args[64][64], int count)
{
   int i;
   char **formatted_args = (char **)calloc(count + 1, sizeof(char *));
   for(i = 0; i < count; i++)
   {
      formatted_args[i] = args[i];
   }
   formatted_args[i] = NULL;

   return formatted_args;
}

void print_cmd(struct command *cmd)
{
   int i;
   printf("\n--------\n");
   printf("Stage %d: \" %s \"\n", cmd->stage, cmd->name);
   printf("--------\n");
   printf("     input: %d\n", cmd->input);
   printf("    output: %d\n", cmd->output);
   printf("      argc: %d\n", cmd->argc);
   printf("      argv: \"%s\"", cmd->args[0]);
   for(i = 1; i < 64; i++)
   {
      if(strcmp(cmd->args[i], "") != 0)
      {
         printf(",\"%s\"", cmd->args[i]);
      }
      else
      {
         break;
      }
   }
   printf("\n");
}

void free_cmds(struct command **cmd_arr)
{
   int i;
   for(i = 0; i < 10; i++)
   {
      free(cmd_arr[i]);
   }
}
