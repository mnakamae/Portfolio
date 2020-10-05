#ifndef __FILESYS_H
#define __FILESYS_H

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>


#define PROG_MINLS 0              /* flag specifying current prog is minls*/
#define PROG_MINGET 1             /* flag specifying current prog is minget*/

#define INODE_0 0                 /* zeroth inode*/
#define ROOT_INODE 1              /* root inode*/
#define SUPER_LOC 1024            /* location of superblock*/
#define SECTOR_SIZE 512
#define LOCATION_PART_TABLE 0x1BE /* location of partition table*/
#define MIN_PART 0x81

#define BYTE_510_VALID 0xAA55     /* used to validate partition table*/
#define BYTE_510_OFFSET 510       /* used to find byte 510*/
#define MAGIC_NUM 0x4D5A          /* used to validate superblock*/
#define MAGIC_NUM_REVERSED 0x5A4D /* used to validate superblock*/

#define INODE_SIZE 64
#define DIR_ENTRY_SIZE 64
#define ONE_STRUCT 1             /* used to fwrite or fread 1 struct*/
#define NULL_CHAR 1              /* used to pad \0 to the end of strcpy*/

/* checks if type is of reg*/
#define MIN_ISREG(m) (((m)&0170000)==0100000)

/* checks if type is of directory*/
#define MIN_ISDIR(m) (((m)&0170000)==0040000)

/* checks if type is of symbolic link*/
#define MIN_ISSYM(m) (((m)&0170000)==0120000)

#define SYM_LINK 0120000       /* Symbolic Link Mask*/
#define FILE_TYPE_MASK 0170000 /* File type mask*/
#define REG_FILE_MASK 0100000  /* Regular file*/
#define DIR_MASK 0040000       /* Directory*/
#define OWR_R 0000400          /* Owner read permission*/
#define OWR_W 0000200          /* Owner write permission*/
#define OWR_X 0000100          /* Owner execute permission*/
#define GRP_R 0000040          /* Group read permission*/
#define GRP_W 0000020          /* Group write permission*/
#define GRP_X 0000010          /* Group execute permission*/
#define OTR_R 0000004          /* Other read permission*/
#define OTR_W 0000002          /* Other write permission*/
#define OTR_X 0000001          /* Other execute permission*/

#define DIRECT_ZONES 7         /* number of direct zones in an inode*/

#define BAD_FSEEK 1      /* exit due to invalid fseek*/
#define BAD_FREAD 2      /* exit due to invalid fread*/
#define BAD_PTABLE 3     /* exit due to bad partition table*/
#define NOT_MINIX_PART 4 /* exit due to bad partition*/
#define BAD_MAGIC_NUM 5  /* exit due to bad superblock*/
#define BAD_TYPE 6       /* exit due to bad non directory or reg*/
#define BAD_PATH 7       /* exit due to invalid path*/

/* used to hold the contents of a partition*/
typedef struct part 
{
   uint8_t    bootind;
   uint8_t    start_head;
   uint8_t    start_sec;
   uint8_t    start_cyl;
   uint8_t    type;
   uint8_t    end_head;
   uint8_t    end_sec;
   uint8_t    end_cyl;
   uint32_t   lFirst;
   uint32_t   size;
} part;

/* used to hold the contents of a directory entry*/
typedef struct dir {
   uint32_t inode;
   unsigned char name[60];
} dir;

/* used to hold the contents of an inode*/
typedef struct inode {
   uint16_t    mode;
   uint16_t    links;
   uint16_t    uid;
   uint16_t    gid;
   uint32_t    size;
   uint32_t     atime;
   uint32_t     mtime;
   uint32_t     ctime;
   uint32_t    zone[DIRECT_ZONES];
   uint32_t    indirect;
   uint32_t    two_indirect;
   uint32_t    unused;
} inode;

/* used to hold the contents of a superblock*/
typedef struct superblock {
   uint32_t    ninodes;
   uint16_t    pad1;
   int16_t     i_blocks;
   int16_t     z_blocks;
   uint16_t    firstdata;
   int16_t     log_zone_size;
   int16_t     pad2;
   uint32_t    max_file;
   uint32_t    zones;
   int16_t     magic;
   int16_t     pad3;
   uint16_t    blocksize;
   uint8_t     subversion;
} superblock;

typedef struct minvars {
   uint32_t zonesize; /* holds the zonesize found in the superblock*/
   part party;        /* holds the specified partition*/
   part sub_party;    /* holds the specified subpartition*/
   superblock superb; /* holds the superblock */
   uint32_t partition_start; /* offset caused by a partition */
} minvars;

/* FUNCTION PROTOTYPES*/
int get_partition(FILE *fp, uint8_t num, minvars *var);
void get_super(FILE *fp, minvars *var);
void print_super(minvars *var);
void get_inode(FILE *fp, inode *dest, uint16_t num, minvars *var);
void print_inode(inode *node);
void print_perms(uint16_t mode);
void print_zones(inode *oop);
void get_dirent(FILE *fp, inode *in, minvars *var );
void get_subpart(FILE *fp, uint8_t num, minvars *var);
void parse_path(FILE *fp, inode *nod, char *path, minvars *var);
void print_file(inode *beth, char *path);
void get_data(FILE *fp, inode *ino, FILE *out, minvars *var);
void get_indirent(FILE *fp, uint32_t zone, minvars *var );

#endif /* __FILESYS_H */