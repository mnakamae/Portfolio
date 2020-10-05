#include <sys/stat.h>
#include <arpa/inet.h>

struct ustar_header
  {
    char name[100];             /* File name.  Null-terminated if room. */
    char mode[8];               /* Permissions as octal string. */
    char uid[8];                /* User ID as octal string. */
    char gid[8];                /* Group ID as octal string. */
    char size[12];              /* File size in bytes as octal string. */
    char mtime[12];             /* Modification time in seconds
                                   from Jan 1, 1970, as octal string. */
    char chksum[8];             /* Sum of octets in header as octal string. */
    char typeflag;              /* An enum ustar_type value. */
    char linkname[100];         /* Name of link target.
                                   Null-terminated if room. */
    char magic[6];              /* "ustar\0" */
    char version[2];            /* "00" */
    char uname[32];             /* User name, always null-terminated. */
    char gname[32];             /* Group name, always null-terminated. */
    char devmajor[8];           /* Device major number as octal string. */
    char devminor[8];           /* Device minor number as octal string. */
    char prefix[155];           /* Prefix to file name.
                                   Null-terminated if room. */
    char padding[12];           /* Pad to 512 bytes. */
  };

void usage();

void create_header_file(int, char*, struct stat*, struct ustar_header*);

void create_name_prefix(char*, struct ustar_header*);

void copy_contents(int, int);

void clear_header(struct ustar_header*);

void clear_string(char*, int);

void get_chksum(struct ustar_header*);

void pad_file(int);

void write_header(int,struct ustar_header*);

int check_flags_fcvtxs(char*);

void dirwalk(char*, int, struct ustar_header*, int);

void recurse_dir(char*, int,  struct ustar_header*, int, void(*fcn)(char *, int, struct ustar_header*, int));

void make_table(int, int);

int insert_special_int(char*, size_t, int32_t);

