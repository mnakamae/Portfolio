#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parse.h"
#include <string.h>
#include "filesys.h"

/* this file contains the main program for minget */

int main (int argc, char **argv)
{
   FILE *fp, *out = 0;
   inode in;
   char *pathcop;
   minvars min;
   flags flag = {0,0,0,0,0,0};

   /* populate flags based on user arguments*/
   parse(argc, argv, &flag, PROG_MINGET);

   /* if path is specified save a safe copy of the pathname*/
   if (flag.path)
   {
      pathcop = malloc(sizeof(char) * (strlen(flag.path) + NULL_CHAR));
      pathcop = strcpy( pathcop , flag.path);
   }
   fp = fopen(flag.file, "r");
   
   /* if partition specified*/
   if (flag.p)
   {
      get_partition(fp, flag.pval, &min);
   }

   /* if subpartition specified*/
   if(flag.s)
   {
      get_subpart(fp, flag.sval, &min);
   }

   /* get superblock and root inode*/
   get_super(fp, &min);
   get_inode(fp, &in, ROOT_INODE, &min);

   /* if path is specified, print that paths contents*/
   if(flag.path)
   {
      parse_path(fp, &in, pathcop, &min);
   }

   /* if verbose specified print out contents of superblock and root inode*/
   if (flag.v)
   {
      print_super(&min);
      print_inode(&in);
      print_zones(&in);
   }

   /* open destination file to output*/
   if(flag.dest)
   {
      out = fopen(flag.dest,"w");
   }

   /* cannot output to a directory*/
   if(MIN_ISDIR(in.mode))
   {
      fprintf(stderr, "this is a directory not a file\n");
      exit(BAD_TYPE);
   }
   /* print out message from specified inode*/
   else if (MIN_ISREG(in.mode))
   {
      if (in.size == 0)
      {
         return 0;
      }
      get_data(fp, &in, out, &min);
   }
   /* neither a directory or reg specified so this is invalid*/
   else
   {
      fprintf(stderr, "this is not a reg or dir\n");
      exit(BAD_TYPE);
   }

   /* free allocated memory and close file pointers*/
   free(pathcop);
   fclose(fp);
   if(flag.dest)
   {
      fclose(out);
   }

   return 0;
}