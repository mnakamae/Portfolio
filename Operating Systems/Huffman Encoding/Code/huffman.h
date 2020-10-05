#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

/* STRUCT TYPES */
struct node
{
  int frequency;
  unsigned char value;
  struct node *next;
  struct node *right;
  struct node *left;
};

struct LinkedList
{
  struct node *head;
  int node_count;
};

/* FUNCTION PROTOTYPES */
struct node * create_node(int freq, unsigned char c);
struct node * create_parent(int freq, struct node *left, struct node *right);
int count_nodes(int *ht);
struct node ** ht_elements(int *ht, int size);
int compare(struct node *n1, struct node *n2);
void insert(struct LinkedList *LL, struct node *new);
struct node * remove_head(struct LinkedList *LL);
struct LinkedList * create_LL(struct node **nodes, int size);
struct node * create_tree(struct LinkedList *LL);
int is_leaf(struct node *n);
void encode(struct node *root, char *string, char **codes, int END_CHAR);
void print_codes(char **codes);
void write_codes(int *ht, char **codes, char *in_file, char *out_file, int count);
int * getwords(char *pathname);
struct node ** rebuild_nodes(char *pathname);
int node_counter(char *pathname);
void rewrite(char *file_in, char *file_out, struct node *head);
void printLL(struct LinkedList *LL);
void free_codes(char **codes);
void free_tree(struct node *root);
