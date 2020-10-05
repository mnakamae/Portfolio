#ifndef __PARSE_H
#define __PARSE_H

#include <stdint.h>

/* struct used to hold parsed user arguments*/
typedef struct flags
{
   uint8_t v;    /* verbose flag*/
   uint8_t p;    /* partition flag*/
   uint8_t pval; /* partition specified*/
   uint8_t s;    /* subpartition flag*/
   uint8_t sval; /* subpartition specified*/
   char *file;   /* image specified*/
   char *path;   /* directory or reg path specified*/
   char *dest;   /* output specified*/
} flags;

int parse (int argc, char **argv, flags *steve, uint8_t prog);

#endif /* __PARSE_H */