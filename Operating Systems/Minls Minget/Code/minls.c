#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parse.h"
#include <string.h>
#include "filesys.h"

/* this file contains the main program for minls */

int main (int argc, char **argv)
{
   FILE *fp;
   inode in;
   char *pathcop;
   minvars min;
   flags flag = {0,0,0,0,0,0};

   /* populate flags struct based on user arguments*/
   parse(argc, argv, &flag, PROG_MINLS);

   /* save a safe copy of the path*/
   if (flag.path)
   {
      pathcop = malloc(sizeof(char) * (strlen(flag.path) + NULL_CHAR));
      pathcop = strcpy(pathcop, flag.path);
   }

   fp = fopen(flag.file, "r");
   
   /* find the partition if specified by user*/
   if (flag.p)
   {
      get_partition(fp, flag.pval, &min);
   }

   /* find the subpartition if specified by user*/
   if(flag.s)
   {
      get_subpart(fp, flag.sval, &min);
   }

   /* get the superblock and the root inode*/
   get_super(fp, &min);
   get_inode(fp, &in, ROOT_INODE, &min);

   /* traverse path specified by user*/
   if(flag.path)
   {
      parse_path(fp, &in, pathcop, &min);
   }

   /* print out superblock and inode data */
   if (flag.v)
   {
      print_super(&min);
      print_inode(&in);
      print_zones(&in);
   }

   /* check ot see if this inode is a directory*/
   if(MIN_ISDIR(in.mode))
   {
      /* print out directory name*/
      if(!flag.path)
      {
         printf("/:\n");
      }
      else
      {
         printf("%s:\n", flag.path);
      }

      /* print out directory contents*/
      get_dirent(fp, &in, &min);

      /* print indirect zones contents if they exist*/
      if(in.indirect)
      {
         get_indirent(fp, in.indirect, &min);
      }
   }
   /* check to see if this inode is a reg*/
   else if (MIN_ISREG(in.mode))
   {
      /* print out the reg */
      print_file(&in, flag.path);
   }
   /* this file was neither reg nor directory so it is invalid*/
   else
   {
      fprintf(stderr, "this is not a reg or dir\n");
      exit(BAD_TYPE);
   }

   /* free allocated memory and close file pointers*/
   if(flag.path)
   {
      free(pathcop);
   }
   fclose(fp);
   return 0;
}
