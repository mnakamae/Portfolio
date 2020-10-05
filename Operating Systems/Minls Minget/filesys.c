#include "filesys.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* This file contains all the necessary helper functions for minls
 * and minget.
 * */

/* Checks bytes 510 and 511 to see if the partition table is valid*/
void validate_part_table(FILE *img, minvars *var)
{
   uint16_t bytez;

   /* move the file pointer to the 510 bytes in the image*/
   if(0 != fseek(img, BYTE_510_OFFSET + var->partition_start, SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }
   
   /* read in both 510 and 511 bytes at same time*/
   if (!fread(&bytez, sizeof(uint16_t), ONE_STRUCT, img))
   {
      fprintf(stderr,"fread failed in validation");
      exit(BAD_FREAD);
   }

   /* check value read in against the valid value*/
   if (bytez != BYTE_510_VALID)
   {
      fprintf(stderr,"this is not a valid table\n");
      exit(BAD_PTABLE);
   }
}

/* Checks to see if the partition read in is a minix partition*/
void validate_part(part *cool, minvars *var)
{
   if(cool->type != MIN_PART)
   {
      fprintf(stderr, "Partition at %d is not a minix partition\n",
               var->partition_start);
      exit(NOT_MINIX_PART);
   }
}

/* Takes a partitioned image and finds the specified partition*/
int get_partition(FILE *fp, uint8_t num, minvars *var)
{
   /* check to see if there is a valid partition table*/
   validate_part_table(fp, var);

   /* move file pointer to the start of the partition*/
   if(0 != fseek(fp, LOCATION_PART_TABLE + sizeof(part) * num , SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }

   /* read in the partition to the global var->party partition*/
   if(!fread(&var->party, sizeof(part), ONE_STRUCT, fp))
   {
      fprintf(stderr,"fread on partition failed");
      exit(BAD_FREAD);
   }
   
   /* check to see if the partition read is valid*/
   validate_part(&var->party, var);

   /* change the offset of the start based on the lFirst of the partition*/
   var->partition_start= SECTOR_SIZE * var->party.lFirst;
   return 0;
}

/* Takes a partitioned image and finds the specified subpartition*/
void get_subpart(FILE *fp, uint8_t num, minvars *var)
{
   /* check to see if there is a valid partition table*/
   validate_part_table(fp, var);

   /* move file pointer to the start of the subpartition*/
   if(0 != fseek(fp, LOCATION_PART_TABLE + sizeof(part) * num +
            var->partition_start, SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }

   /* read in the subpartition to the global sub_var->party partition*/
   if(!fread(&(var->sub_party), sizeof(part), ONE_STRUCT, fp))
   {
      fprintf(stderr,"fread on partition failed");
      exit(BAD_FREAD);
   }

   /* change the offset of the start based on the lFirst of the partition*/
   var->partition_start = SECTOR_SIZE * var->sub_party.lFirst;

   /* check to see if the subpartition read is valid*/
   validate_part(&(var->sub_party), var);  
}

/* Takes an image and returns the superblock*/
void get_super(FILE *fp, minvars *var)
{
   /* move file pointer to start of superblock*/
   if(0 != fseek(fp, SUPER_LOC + var->partition_start, SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }

   /* read in the superblock into the global superb superblock*/
   if(!fread(&(var->superb), sizeof(superblock), ONE_STRUCT, fp))
   {
      fprintf(stderr,"fread on partition failed");
      exit(BAD_FREAD);
   }

   /* check the magic number to see if this is a valid superblock*/
   if(MAGIC_NUM != var->superb.magic)
   {
      fprintf(stderr,"Bad magic number. (0x%.4x)\n", var->superb.magic);
      fprintf(stderr,"This doesn\'t look like a MINIX filesystem.\n");
      exit(BAD_MAGIC_NUM);
   }

   /* set the global zonesize variable*/
   var->zonesize = var->superb.blocksize << var->superb.log_zone_size;
}

/* Takes an image and returns the specified inode in the dest buffer*/
void get_inode(FILE *fp, inode *dest, uint16_t num, minvars *var)
{
   /* invalid if inode is the 0th inode*/
   if (num == INODE_0)
   {
      fprintf(stderr,"0th inode is invalid\n");
      return;
   }

   /* move file pointer to the start of the specified inode*/
   if( 0 != fseek(fp,
        ((2+var->superb.i_blocks + var->superb.z_blocks) * 
        var->superb.blocksize) + (num - 1) * sizeof(inode) + 
        var->partition_start, SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }

   /* read in this inode to the dest buffer*/
   if (!fread(dest, sizeof(inode), ONE_STRUCT, fp))
   {
      fprintf(stderr,"fread on partition failed");
      exit(BAD_FREAD);
   }
}

/* prints directory entries from specified inode */
void get_dirent(FILE *fp, inode *in, minvars *var )
{
   dir erect;
   uint32_t i = 0, j = 0;

   /* loop through each direct zone */
   for(j = 0; j < DIRECT_ZONES; j++)
   {
      if (in->zone[j] != 0)
      {
         i = 0;
	 /* for each entry in the direct zone */
         while(i < var->zonesize / sizeof(dir))
         {
            inode curr;
	    /* move file pointer to the dir entry */
            if( 0 != fseek(fp,
                  (in->zone[j] * var->zonesize) +
                  i * sizeof(dir) + var->partition_start, SEEK_SET))
            {
               fprintf(stderr,"fseek");
               exit(BAD_FSEEK);
            }
	    /* read in the dir entry */
            if (!fread(&erect, sizeof(dir), ONE_STRUCT, fp))
            {
               fprintf(stderr,"fread on dirent");
               exit(BAD_FREAD);
            }
	    /* print corresponding inode of dir entry */
            if (erect.inode)
            {
               get_inode(fp, &curr, erect.inode, var);
               print_perms(curr.mode);
               printf("   %6d", curr.size);
               printf(" %s\n", erect.name);
            } 
            i++;
         }
      }
   }
}

/* prints contents in the indirect zones */
void get_indirent(FILE *fp, uint32_t zone, minvars *var )
{
   dir erect;
   uint32_t i = 0, j = 0, *arr;
   arr = malloc(var->zonesize);

   /* move file pointer to the start of the indirect zone */
   if(0 != fseek(fp,
         (zone * var->zonesize) + var->partition_start, SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }

   /* read in the direct numbers in the indirect zone */
   if (!fread(arr, var->zonesize, ONE_STRUCT, fp))
   {
      fprintf(stderr,"fread on dirent");
      exit(BAD_FREAD);
   }

   /* for each direct zone number */
   for(j = 0; j < var->zonesize / sizeof(uint32_t); j++)
   {
      /* print out nonzero contents */
      if (arr[j] != 0)
      {
         i = 0;
	 /* for each dir entry in the direct zone */
         while(i < var->zonesize / sizeof(dir))
         {
            inode curr;

	    /* move file pointer to dir entry and read it in */
            if( 0 != fseek(fp,
                  (arr[j] * var->zonesize) +
                  i * sizeof(dir) + var->partition_start, SEEK_SET))
            {
               fprintf(stderr,"fseek");
               exit(BAD_FSEEK);
            }
            if (!fread(&erect, sizeof(dir), ONE_STRUCT, fp))
            {
               fprintf(stderr,"fread on dirent");
               exit(BAD_FREAD);
            }

	    /* print out the entry if it has a valid inode */
            if (erect.inode)
            {
               get_inode(fp, &curr, erect.inode, var);
               print_perms(curr.mode);
               printf("   %6d", curr.size);
               printf(" %s\n", erect.name);
            } 
            i++;
         }
      }
   }
   free(arr);
}


void parse_indirect(FILE* fp, inode *nod, char *toke, uint8_t *flag,
   minvars *var)
{
   dir curr_dir;
   uint32_t i = 0, j = 0, *arr;
   arr = malloc(var->zonesize);

   /* find the indirect zone and read in the numbers */
   if( 0 != fseek(fp,
         (nod->indirect * var->zonesize) + var->partition_start, SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }
   if (!fread(arr, var->zonesize, ONE_STRUCT, fp))
   {
      fprintf(stderr,"fread on dirent");
      exit(BAD_FREAD);
   }

   /* for each direct zone */
   for(j=0; j < (var->zonesize / sizeof(uint32_t)); j++)
   {
      /* print out the non zero zones */
      if (arr[j] != 0)
      {
         i = 0;

	 /* for each dir entry in the direct zone */
         while(i < (var->zonesize / sizeof(dir)))
         {
	    /* find and read in the dir entry */
            if(0 != fseek(fp,
                 (arr[j] * var->zonesize) +
                 i * sizeof(dir) + var->partition_start, SEEK_SET))
            {
               fprintf(stderr, "fseek");
               exit(BAD_FSEEK);
            }
            if (!fread(&curr_dir, sizeof(dir), ONE_STRUCT, fp))
            {
               fprintf(stderr, "fread on dirent");
               exit(BAD_FREAD);
            }

	    /* if the inode is valid and is the specified path, print */
            if (curr_dir.inode)
            {
               if (!strcmp(toke, (char *)curr_dir.name))
               {
                  get_inode(fp, nod, curr_dir.inode,var);
                  if ((!(MIN_ISDIR(nod->mode) || MIN_ISREG(nod->mode))))
                  {
                     fprintf(stderr, "not a directory or a file");
                     exit(BAD_TYPE);
                  }
                  *flag = 1;
                  free(arr);
                  return;
               }
            } 
            i++;
         }
      }
   }
   free(arr);
}

/* takes a specified path and prints out its contents */
void parse_path(FILE *fp, inode *nod, char *path, minvars *var)
{
   dir curr_dir;
   uint32_t i = 0, j = 0;
   uint8_t flag = 0;
   char *toke;
   toke = strtok(path,"/");

   /* while still traversing the path */
   while( toke != NULL)
   {
      if (MIN_ISREG(nod->mode))
      {
         fprintf(stderr, "Cannot go any deeper\n");
         exit(1);
      }
      flag = 0;

      /* check the subdirectory's contents to traverse deeper */
      for(j = 0; j < DIRECT_ZONES; j++)
      {
         if (nod->zone[j] != 0)
         {
            i = 0;

	    /* check each entry to see if it is the correct path */
            while( i < var->zonesize  / sizeof(dir))
            {
               if( 0 != fseek(fp,
                     (nod->zone[j] * var->zonesize) +
                     i * sizeof(dir) + var->partition_start, SEEK_SET))
               {
                  fprintf(stderr,"fseek");
                  exit(BAD_FSEEK);
               }
               if (!fread(&curr_dir, sizeof(dir), ONE_STRUCT, fp))
               {
                  fprintf(stderr,"fread on dirent");
                  exit(BAD_FREAD);
               }

               if (curr_dir.inode)
               {
		  /* correct path is found, print contents */
                  if (!strcmp(toke,(char *)curr_dir.name))
                  {
                     get_inode(fp, nod, curr_dir.inode, var);
                     if ((!(MIN_ISDIR(nod->mode) || MIN_ISREG(nod->mode))))
                     {
                        fprintf(stderr, "not a directory or a file");
                        exit(BAD_TYPE);
                     }
                     flag = 1;
                  }
               } 
               i++;
            }
         }
      }
      /* continue traversing down path recursively */
      if (!flag)
      {
         parse_indirect(fp, nod, toke,  &flag, var);
      }
      if (!flag)
      {
         fprintf(stderr, "Bad Path\n");
         exit(BAD_PATH);
      }
      toke = strtok(NULL, "/");
   }

}

/* finds and prints the data stored in the double indirect zone */
void get_2data(FILE *fp, inode *ino, FILE *out, 
   uint32_t *sizread, char *data, minvars *var)
{
   uint32_t f=0, z=0, *crap, sizto, *indir; 
   char *zeroz;
   crap = malloc(var->zonesize);
   indir = malloc(var->zonesize);
   zeroz = calloc(var->zonesize * var->zonesize, sizeof(char));
   
   /* find and read in the double indirect zone */
   if( 0 != fseek(fp,
         (ino->two_indirect * var->zonesize) + var->partition_start, SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }
   if (!fread(crap, var->zonesize, ONE_STRUCT, fp))
   {
      fprintf(stderr,"fread on dirent 1 in indir");
      exit(BAD_FREAD);
   }

   /* for each of the indirect zones in the double indirect zone */
   for(f = 0; f < var->zonesize / sizeof(uint32_t); f++)
   {
      /* wrote enough to output, return */
      if (*sizread >= ino->size)
      {
         free(crap);
         return;
      }

      /* zero zone is found, write an indirect zones worth of zeros */
      if (!crap[f])
      {
         sizto = (var->zonesize * var->zonesize / 4 >= ino->size - *sizread) ? 
            ino->size - *sizread : var->zonesize * var->zonesize / 4;
         if (out)
         {
            fwrite(zeroz, sizto, 1, out);
         }
         else
         {
            fwrite(zeroz, sizto, 1, stdout);
         }
         *sizread += var->zonesize * var->zonesize/4;
         continue;
      }

      /* find and read in each indirect zone */
      if( 0 != fseek(fp,
            (crap[f]* var->zonesize) + var->partition_start, SEEK_SET))
      {
         fprintf(stderr,"fseek");
         exit(BAD_FSEEK);
      }
      if (!fread(indir, var->zonesize, ONE_STRUCT, fp))
      {
         fprintf(stderr,"fread on dirent in indir");
         exit(BAD_FREAD);
      }

      /* for each direct zone in the indirect zone */
      for (z = 0; z <( var->zonesize / sizeof(uint32_t)); z++)
      {
	 /* when a zero is found write a direct zones worth of zero */
         if(!indir[z])
         {
            memset(data, 0, var->zonesize);
            sizto = var->zonesize;
         }
         else
         {
            if( 0 != fseek(fp,(indir[z] * var->zonesize) +
               var->partition_start, SEEK_SET))
            {
               fprintf(stderr,"fseek");
               exit(BAD_FSEEK);
            }
            sizto = (var->zonesize >= ino->size - *sizread) ? 
               ino->size - *sizread : var->zonesize;
            if (!fread(data, sizto, ONE_STRUCT, fp))
            {
               fprintf(stderr,"fread on dirent it is me");
               exit(BAD_FREAD);
            }
         }
         *sizread += sizto;

	 /* write data from direct zones to output */
         if(out)
         {
            fwrite(data, sizto, ONE_STRUCT, out);
         }
         else
         {
            fwrite(data, sizto, ONE_STRUCT, stdout);
         }

	 /* enough written to output, return */
         if (*sizread >= ino->size)
         {
            free(crap);
            return;
         }
      }
   }
   free(crap);
}

/* finds and outputs the data found in the indirect zones */
void get_indata(FILE *fp, inode *ino, FILE *out, uint32_t *sizread, 
   char *data, minvars *var)
{
   uint32_t  j = 0, *arr, sizto ;
   arr = malloc(var->zonesize);

   /* find and read in the indirect zone */
   if( 0 != fseek(fp,
         (ino->indirect * var->zonesize) + var->partition_start, SEEK_SET))
   {
      fprintf(stderr,"fseek");
      exit(BAD_FSEEK);
   }
   if (!fread(arr, var->zonesize, ONE_STRUCT, fp))
   {
      fprintf(stderr,"fread on dirent in in1");
      exit(BAD_FREAD);
   }

   /* for each direct zone in the indirect zone */
   for(j = 0; j < (var->zonesize / sizeof(uint32_t)); j++)
   {
      /* write a zone of zeros when an empty direct zone found */
      if(!arr[j] || !(ino->indirect))
      {
         memset(data, 0, var->zonesize);
         sizto = var->zonesize;
      }
      else
      {
	 /* find and read in the data to be output */
         if( 0 != fseek(fp,(arr[j] * var->zonesize) + var->partition_start, 
            SEEK_SET))
         {
            fprintf(stderr,"fseek");
            exit(BAD_FSEEK);
         }
         sizto = (var->zonesize >= ino->size - *sizread) ? 
            ino->size - *sizread : var->zonesize;
         
         if (!fread(data, sizto, ONE_STRUCT, fp))
         {
            fprintf(stderr,"fread on dirent in 2");
            exit(BAD_FREAD);
         }
      }
      *sizread += sizto;

      /* write the data to the output */
      if(out)
      {
         fwrite(data, sizto, ONE_STRUCT, out);
      }
      else
      {
         fwrite(data, sizto, ONE_STRUCT, stdout);
      }

      /* written enough to the output, return*/
      if (*sizread >= ino->size)
      {
         free(arr);
         return;
      }
   }
   free(arr);
}

/*  */
void get_data(FILE *fp, inode *ino, FILE *out, minvars *var)
{
   char *data;
   uint32_t  j = 0, sizread = 0, sizto = 0;
   data = malloc(var->zonesize * sizeof(char));

   /* for each direct zone */
   for(j = 0; j < DIRECT_ZONES; j++)
   {
      /* find and read in the data to be output */
      if( 0 != fseek(fp,
            (ino->zone[j] * var->zonesize) +
            var->partition_start, SEEK_SET))
      {
         fprintf(stderr,"fseek");
         exit(BAD_FSEEK);
      }
      sizto = (var->zonesize >= ino->size-sizread)? 
         ino->size-sizread : var->zonesize;
      if (!fread(data, sizto, ONE_STRUCT, fp))
      {
         fprintf(stderr,"fread on dirent");
         exit(BAD_FREAD);
      }
      sizread += sizto;

      /* zero zone encountered, output a zones worth of zeros*/
      if(ino->zone[j] == 0)
      {
         memset(data, 0, var->zonesize);
      }

      /* write to output */
      if(out)
      {
         fwrite(data, sizto, ONE_STRUCT, out);
      }
      else
      {
         fwrite(data, sizto, ONE_STRUCT, stdout);

      }

      /* written enough to output, return */
      if (sizread >= ino->size)
      {
         free(data);
         return;
      }
   }

   /* not enough written to output, check other zones for data */
   get_indata(fp, ino, out, &sizread, data, var);
   get_2data(fp, ino, out, &sizread, data, var);
   free(data);
}

/* prints out the permissions, size, and name for a file */
void print_file(inode *beth, char *path)
{
   print_perms(beth->mode);
   printf(" %9d", beth->size);
   printf(" %s\n", path);
}

/* prints out the contents of a super */
void print_super(minvars *var)
{
   fprintf(stderr,"\nSuperblock Contents:\n");
   fprintf(stderr,"Stored Fields:\n");
   fprintf(stderr,"  ninodes          %d\n", var->superb.ninodes);
   fprintf(stderr,"  i_blocks           %d\n", var->superb.i_blocks);
   fprintf(stderr,"  z_blocks           %d\n", var->superb.z_blocks);
   fprintf(stderr,"  firstdata         %d\n", var->superb.firstdata);
   fprintf(stderr,"  log_zone_size      %d  (zone size: %d)\n",
                           var->superb.log_zone_size, var->zonesize);
   fprintf(stderr,"  max_file  %u\n", var->superb.max_file);
   fprintf(stderr,"  magic         0x%x\n", var->superb.magic);
   fprintf(stderr,"  zones            %d\n", var->superb.zones);
   fprintf(stderr,"  blocksize       %d\n", var->superb.blocksize);
   fprintf(stderr,"  subversion         %d\n\n", var->superb.subversion);
}

/* prints out the permissions of an inode to stdout */
void print_perms(uint16_t mode)
{
   printf("%c", ((mode &  DIR_MASK)  ? 'd' : '-'));
   printf("%c", ((mode &  OWR_R)  ? 'r' : '-'));
   printf("%c", ((mode &  OWR_W)  ? 'w' : '-'));
   printf("%c", ((mode &  OWR_X)  ? 'x' : '-'));
   printf("%c", ((mode &  GRP_R)  ? 'r' : '-'));
   printf("%c", ((mode &  GRP_W)  ? 'w' : '-'));
   printf("%c", ((mode &  GRP_X)  ? 'x' : '-'));
   printf("%c", ((mode &  OTR_R)  ? 'r' : '-'));
   printf("%c", ((mode &  OTR_W)  ? 'w' : '-'));
   printf("%c", ((mode &  OTR_X)  ? 'x' : '-'));
}

/* prints out the permissions of an inode to stderr */
void fprint_perms(uint16_t mode)
{
   fprintf(stderr,"%c", ((mode &  DIR_MASK)  ? 'd' : '-'));
   fprintf(stderr,"%c", ((mode &  OWR_R)  ? 'r' : '-'));
   fprintf(stderr,"%c", ((mode &  OWR_W)  ? 'w' : '-'));
   fprintf(stderr,"%c", ((mode &  OWR_X)  ? 'x' : '-'));
   fprintf(stderr,"%c", ((mode &  GRP_R)  ? 'r' : '-'));
   fprintf(stderr,"%c", ((mode &  GRP_W)  ? 'w' : '-'));
   fprintf(stderr,"%c", ((mode &  GRP_X)  ? 'x' : '-'));
   fprintf(stderr,"%c", ((mode &  OTR_R)  ? 'r' : '-'));
   fprintf(stderr,"%c", ((mode &  OTR_W)  ? 'w' : '-'));
   fprintf(stderr,"%c", ((mode &  OTR_X)  ? 'x' : '-'));
}

/* prints out the contents of an inode */
void print_inode(inode *node)
{
   time_t a=node->atime ,m=node->mtime,c=node->ctime;
   fprintf(stderr,"File inode:\n");
   fprintf(stderr,"  unsigned short mode         0x%.4x    (",  node->mode);
   fprint_perms(node->mode);
   fprintf(stderr,")\n  unsigned short links          %4d\n", node->links);
   fprintf(stderr,"  unsigned short uid            %4d\n", node->uid);
   fprintf(stderr,"  unsigned short gid            %4d\n", node->gid);
   fprintf(stderr,"  uint32_t  size         %6d\n", node->size);
   fprintf(stderr,"  uint32_t  atime    %10d --- %s", node->atime, ctime(&a));
   fprintf(stderr,"  uint32_t  mtime    %10d --- %s", node->mtime, ctime(&m));
   fprintf(stderr,"  uint32_t  ctime    %10d --- %s\n",node->ctime,ctime(&c));
}

/* prints out the values contained in the zones */
void print_zones(inode *oop)
{
   fprintf(stderr,"  Direct zones:\n");
   fprintf(stderr,"              zone[0]   =      %5d\n", oop->zone[0]);
   fprintf(stderr,"              zone[1]   =      %5d\n", oop->zone[1]);
   fprintf(stderr,"              zone[2]   =      %5d\n", oop->zone[2]);
   fprintf(stderr,"              zone[3]   =      %5d\n", oop->zone[3]);
   fprintf(stderr,"              zone[4]   =      %5d\n", oop->zone[4]);
   fprintf(stderr,"              zone[5]   =      %5d\n", oop->zone[5]);
   fprintf(stderr,"              zone[6]   =      %5d\n", oop->zone[6]);
   fprintf(stderr,"   uint32_t  indirect   =      %5d\n", oop->indirect);
   fprintf(stderr,"   uint32_t  double     =      %5d\n", oop->two_indirect);
}