#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include "funcs.h"

#define SIZE 256

int main(int argc, char *argv[]){
   int tarfile = 0, flags = 0, i = 0;
   struct ustar_header header;
   char path[SIZE];

   if(argc > 1)
      flags = check_flags_fcvtxs(argv[1]);

   if(argc < 3 || (flags & 1) == 0){ /* if less than 3 args */
      usage();                       /* or f flag not included, usage error */
   }                                                    

   if(flags & 2){ /* if c flag, create tarfile */
      tarfile = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0666);
      if(tarfile < 0){
         usage();
      }
      if(argc == 3){
         getcwd(path, SIZE);
         dirwalk(path, tarfile, &header, flags);
      }
      else{
         for(i = 3; i < argc; i++){
            dirwalk(argv[i], tarfile, &header, flags);
         }
         pad_file(tarfile);
      }
   }

   else if(flags & 8){ /* if t flag */
      tarfile = open(argv[2], O_RDONLY);
      make_table(tarfile, flags);
   }

   close(tarfile);
   return 0;
}
