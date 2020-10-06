#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <arpa/inet.h>
#include "dirent.h"
#include "funcs.h"
#include "time.h"

#define SIZE 256

void usage() {
   printf("Usage: mytar [ctxv]f tarfile [path [...] ]\n");
   exit(-1);
}

void create_header_file(int tfile, char *pathname, struct stat *buf, struct ustar_header *header){
   mode_t m = buf->st_mode;
   struct passwd *usr;
   struct group *grp;
   int err = 0;

   create_name_prefix(pathname, header);

   sprintf(header->mode, "%07ho", (unsigned short) (buf->st_mode & 0x1ff)); /* write mode */

   err = insert_special_int(header->uid, 7, buf->st_uid);

   sprintf(header->gid, "%07o", buf->st_gid); /* write gid */

   if(S_ISREG(m)){ /* write size and typeflag */
      sprintf(header->size, "%011lo", (long) buf->st_size); /* write size of regular file */
      header->typeflag = '0'; /* set regular file typeflag */
   }
   else{
      sprintf(header->size, "%011lo", (unsigned long)0); /* write 0 if sym link or dir */
      if(S_ISDIR(m))
         header->typeflag = '5'; /* set dir typeflag */
      else
         header->typeflag = '2'; /* set sym link typeflag */
   }
   /*write(tfile, header->size, sizeof(char) * 12);*/

   sprintf(header->mtime, "%lo", (long) buf->st_mtime); /* write mtime */
   /*write(tfile, header->mtime, sizeof(char) * 12);*/

   strncpy(header->chksum, "        ", sizeof(char) * 8); /* CHECKSUM WOULD BE HERE, but it has to be done a different way later in printheader() */
   /*write(tfile, header->chksum, sizeof(char) * 8);*/

   /*write(tfile, &header->typeflag, sizeof(char));  write typeflag */

   if(S_ISLNK(buf->st_mode)){ /* LINK NAME HERE */
      readlink(pathname, header->linkname, sizeof(char) * 100);
   }
   /*write(tfile, header->linkname, sizeof(char) * 100);*/

   strncpy(header->magic, "ustar", sizeof(char) * 6); /* write magic */
   /*write(tfile, header->magic, sizeof(char) * 6);*/

   header->version[0] = header->version[1] = '0'; /* write version */
   /*write(tfile, header->version, sizeof(char) * 2);*/

   usr = getpwuid(getuid()); /* write uname */
   strcpy(header->uname, usr->pw_name);
   /*write(tfile, header->uname, sizeof(char) * 32);*/

   grp = getgrgid(getgid()); /* write gname */
   strcpy(header->gname, grp->gr_name);

   get_chksum(header);
}

void create_name_prefix(char *pathname, struct ustar_header *header){
   int i = 0, len, j = 0;
   char ret_str[155] = {'\0'}, c, path[256];

   strcpy(path, pathname);
   len = strlen(path);
   if(len < 100)
   {
      while(i < 100){
         if(i < len)
            c = path[i];
         else{
            c = '\0';
         }
         header->name[i] = c;
         /*write(tfile, &c, sizeof(char));*/
         i++;
      }
      return;
   }
   i = len - 100;
   while(i < len && path[i] != '/')
   {
      i++;
   }
   strncpy(ret_str, path, i);
   i++;
   while(i < len){
      header->name[j] = path[i];
      i++;
      j++;
   }
   strcpy(header->prefix, ret_str);
}

void copy_contents(int tfile, int infile){
   char *buf = (char *)calloc(512, sizeof(char));
   int bytes_read;

   while((bytes_read = read(infile, buf, 512 * sizeof(char))) != 0)
   {
      write(tfile, buf, 512 * (sizeof(char)));
      free(buf);
      buf = (char *)calloc(512, sizeof(char));
   }
   free(buf);
}

void clear_header(struct ustar_header *header){
   clear_string(header->name, 100);
   clear_string(header->mode, 8);
   clear_string(header->uid, 8);
   clear_string(header->gid, 8);
   clear_string(header->size, 12);
   clear_string(header->mtime, 12);
   clear_string(header->chksum, 8);
   header->typeflag = '\0';
   clear_string(header->linkname, 100);
   clear_string(header->magic, 6);
   clear_string(header->version, 2);
   clear_string(header->uname, 32);
   clear_string(header->gname, 32);
   clear_string(header->devmajor, 8);
   clear_string(header->devminor, 8);
   clear_string(header->prefix, 155);
   clear_string(header->padding, 12);
}

void clear_string(char *string, int length){
   int i;
   for(i = 0; i < length; i++)
      string[i] = '\0';
}

void get_chksum(struct ustar_header *header){
   char buf[512] = {'\0'};
   int i = 0;
   unsigned int sum = 0;

   strcat(buf, header->name);
   strcat(buf, header->mode);
   strcat(buf, header->uid);
   strcat(buf, header->gid);
   strcat(buf, header->size);
   strcat(buf, header->mtime);
   strcat(buf, header->chksum);
   strcat(buf, header->linkname);
   strcat(buf, header->magic);
   strcat(buf, header->version);
   strcat(buf, header->uname);
   strcat(buf, header->gname);
   strcat(buf, header->devmajor);
   strcat(buf, header->devminor);
   strcat(buf, header->prefix);
   strcat(buf, header->padding);
   while(i < strlen(buf)){
      sum += (unsigned char)buf[i];
      i++;
   }
   sum += header->typeflag;
   clear_string(header->chksum, 8);
   sprintf(header->chksum, "%07o", sum);
}

void write_header(int tfile, struct ustar_header *header){
   write(tfile, header->name, sizeof(char) * 100);
   write(tfile, header->mode, sizeof(char) * 8);
   write(tfile, header->uid, sizeof(char) * 8);
   write(tfile, header->gid, sizeof(char) * 8);
   write(tfile, header->size, sizeof(char) * 12);
   write(tfile, header->mtime, sizeof(char) * 12);
   write(tfile, header->chksum, sizeof(char) * 8);
   write(tfile, &header->typeflag, sizeof(char));
   write(tfile, header->linkname, sizeof(char) * 100);
   write(tfile, header->magic, sizeof(char) * 6);
   write(tfile, header->version, sizeof(char) * 2);
   write(tfile, header->uname, sizeof(char) * 32);
   write(tfile, header->gname, sizeof(char) * 32);
   write(tfile, header->devmajor, sizeof(char) * 8); /* write major number: null */
   write(tfile, header->devminor, sizeof(char) * 8); /* write minor number: null */
   write(tfile, header->prefix, sizeof(char) * 155); /* write prefix */
   write(tfile, header->padding, sizeof(char) * 12);/* write padding */
}

int check_flags_fcvtxs(char *arg)
{
  int i;
  int flags = 0;
  for(i = 0; i < strlen(arg); i++)
  {
    if(arg[i] == 'f')
    {
      flags |= 1;
    }
    else if(arg[i] == 'c')
    {
      flags |= 2;
    }
    else if(arg[i] == 'v')
    {
      flags |= 4;
    }
    else if(arg[i] == 't')
    {
      flags |= 8;
    }
    else if(arg[i] == 'x')
    {
      flags |= 16;
    }
    else if(arg[i] == 's')
    {
      flags |= 32;
    }
  }

  return flags;
}

/* this is a helper function to help dirwalk recurse through subdirectories */
void recurse_dir(char *dir, int tfile,  struct ustar_header *header,int flags, void(*fcn)(char *, int, struct ustar_header*, int))
{
  char name[SIZE];
  struct dirent *dp;
  DIR *dfd;

  if((dfd = opendir(dir)) == NULL)
  {
    fprintf(stderr, "dirwalk: can't open %s\n", dir);
    return;
  }
  while((dp = readdir(dfd)) != NULL)
  {
    if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
    {
      continue;
    }
    if(strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
    {
      fprintf(stderr, "dirwalk: name %s/%s too long\n", dir, dp->d_name);
    }
    else
    {
      sprintf(name, "%s%s", dir, dp->d_name);
      (*fcn)(name, tfile, header, flags);
    }
  }
  closedir(dfd);
}

/* dirwalk takes in the name of a directory and recurses through until it
reaches each subdirectory / file / symbolic link in the root directory */
void dirwalk(char *name, int tfile, struct ustar_header *header, int flags)
{
  struct stat stbuf;
  int infile;

  if(lstat(name, &stbuf) == -1)
  {
    fprintf(stderr, "fsize: can't access %s\n", name);
    return;
  }
  if(S_ISDIR(stbuf.st_mode)) /* FOUND A DIRECTORY */
  {
    strcat(name, "/");
    if(flags & 4){
      printf("%s\n", name);
    }
    clear_header(header);
    create_header_file(tfile, name, &stbuf, header);
    write_header(tfile, header);
    recurse_dir(name, tfile, header, flags, dirwalk);
  }
  else if(S_ISREG(stbuf.st_mode) || S_ISLNK(stbuf.st_mode)) /*FILE OR SYMBOLIC LINK */
  {
    /* CALL TAR FUNCTIONS ON FILES HERE */
    infile = open(name, O_RDONLY);
    if((flags & 4)){
      printf("%s\n", name);
    }
    clear_header(header);
    create_header_file(tfile, name, &stbuf, header);
    write_header(tfile, header);
    copy_contents(tfile, infile); /* only for file, sym link */
    close(infile);
  }
}

void pad_file(int tfile){
   char pad[512] = {'\0'};
   write(tfile, pad, 512);
   write(tfile, pad, 512);
}

void make_table(int tfile, int flags){
   char path[SIZE] = {'\0'}, mode[11], ugname[18], mtime[17];
   int size, i;
   struct ustar_header header;
   struct stat buffer;
   time_t t;
   struct tm lt;

   if(tfile < 0){
      /*error*/
      exit(-1);
   }

   while((size = read(tfile, &header, sizeof(char) * 512)) != 0){
      if(strcmp("ustar", header.magic) == 0){
         if(strlen(header.prefix) > 0){
            strcpy(path, header.prefix);
            strcat(path, "/");
         }
         strcat(path, header.name);
         if(flags & 4){
            stat(path, &buffer);
            strcat(mode, (S_ISDIR(buffer.st_mode)) ? "d" : "-"); /* MODE: mode */
            strcat(mode, (buffer.st_mode & S_IRUSR) ? "r" : "-");
            strcat(mode, (buffer.st_mode & S_IWUSR) ? "w" : "-");
            strcat(mode, (buffer.st_mode & S_IXUSR) ? "x" : "-");
            strcat(mode, (buffer.st_mode & S_IRGRP) ? "r" : "-");
            strcat(mode, (buffer.st_mode & S_IWGRP) ? "w" : "-");
            strcat(mode, (buffer.st_mode & S_IXGRP) ? "x" : "-");
            strcat(mode, (buffer.st_mode & S_IROTH) ? "r" : "-");
            strcat(mode, (buffer.st_mode & S_IWOTH) ? "w" : "-");
            strcat(mode, (buffer.st_mode & S_IXOTH) ? "x" : "-");

            strcat(ugname, header.uname); /* UNAME/GNAME: ugname */
            strcat(ugname, "/");
            strcat(ugname, header.gname);
            i = strlen(ugname);
            while(i < 17){
               ugname[i] = ' ';
               i++;
            }

            /* SIZE: buffer.st_size, %lld */

            t = buffer.st_mtime; /* MTIME: mtime */
            localtime_r(&t, &lt);
            strftime(mtime, sizeof(mtime), "%Y-%m-%d %H:%M", &lt);
            mtime[16] = '\0';

            printf("%s %s%8lld %s ", mode, ugname, buffer.st_size, mtime);
         }
         if(header.prefix[0] != 0)
            printf("%s/", header.prefix);
         printf("%s\n", header.name);
         clear_string(path, SIZE);
         clear_string(mode, 11);
         clear_string(ugname, 18);
         clear_string(mtime, 17);
      }
   }
}

int insert_special_int(char *where, size_t size, int32_t val){
   int err = 0;
   if(val < 0 || (size < sizeof(val)))
   {
      err++;
   }
   else
   {
      printf("%o\n", val);
      printf("%s\n", where);
      memset(where, 0, size);
      *(int32_t *)(where + size - sizeof(val)) = htonl(val);
      *where |= 0x80;
      printf("%s\n", where);
   }
   return err;
}
