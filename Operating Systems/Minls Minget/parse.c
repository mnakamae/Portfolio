#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parse.h"

#define USAGEL "usage: minls [ -v ] [ -p num [ -s num ] ] imagefile [ path ]\n"
#define USEG "usage: minget [ -v ] [ -p num [ -s num ] ] imagefile minixpath \
[ hostpath ]\n"
#define PART "        -p       part    --- select partition for filesystem \
(default: none)\n"
#define SUB  "        -s       sub     --- select subpartition for filesystem \
(default:none)\n"
#define HELP "        -h       help    --- print usage information and exit\n"
#define VERB "        -v       verbose --- increase verbosity level\n"

/* this file contains the helper functions used to parse user input */


/* Prints out a usage error based on the users bad input*/
void end(uint8_t prog)
{

   fprintf(stderr, prog? USEG:USAGEL);
   fprintf(stderr, "Options:\n");
   fprintf(stderr, PART);
   fprintf(stderr, SUB);
   fprintf(stderr, HELP);
   fprintf(stderr, VERB);
   exit(1);
}

/* Takes in the arguments passed in and populates the global flags struct*/
int parse (int argc, char **argv, flags *steve, uint8_t prog)
{
   int index;
   int c;

   opterr = 0;

   while ((c = getopt (argc, argv, "hvp:s:")) != -1)
      switch (c)
      {
         uint32_t part;
         case 'h': /* help flag specified*/
            end(prog);
            break;
         case 'v': /* verbose flag specified*/
            steve-> v = 1;
            break;
         case 'p': /* partitioned flag specified*/
            if( EOF == sscanf(optarg, "%d", &part))
            {
               end(prog);
            }
            steve->p = 1;

            /* invalid partition given*/
            if (part > 3 || part < 0)
            {
               fprintf(stderr,"Partition %d out of range.  Must be 0..3.\n",
                     part);
               end(prog);
            }
            steve->pval = part;
            break;
         case 's': /* subpartition flag specified*/
            if( EOF == sscanf(optarg, "%d", &part))
            {
               end(prog);
            }
            steve->s = 1;

            /* invalid subpartition given*/
            if (part > 3 || part < 0)
            {
               fprintf(stderr,"Partition %d out of range.  Must be 0..3.\n",
                     part);
               end(prog);
            }
            steve->sval = part;
            break;
         case '?':
            /* no (sub)partition specified*/
            if (optopt == 'p' || optopt == 's')
               fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            /* invalid flag given*/
            if (isprint (optopt))
               fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else /* invalid flag given*/
               fprintf (stderr,"Unknown option character `\\x%x'.\n",
                        optopt);
            end(prog);
         default:
            abort ();
      }

   /* determine the path and destinations of the flags struct*/
   c = 0;
   for (index = optind; index < argc; index++)
   {
      if (c == 0)
      {
         steve->file = argv[index];
         c++;
      }
      else if (c == 1)
      {
         steve->path = argv[index];
         c++;
      }
      else if(c == 2 && prog == 1)
      {
         steve->dest = argv[index];
      }
      else
      {
         end(prog);
      }
   }

   /* no file specified*/
   if (steve->file == NULL)
   {
      end(prog);
   }

   return 0;
}