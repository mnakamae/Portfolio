#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* STRUCTURES */
struct node{
  char key;
  struct node *next;
};

struct LinkedList{
  struct node *head;
  int num_nodes;
};


/* FUNCTION PROTOTYPES */
struct node * create_node(char *key);
struct LinkedList * create_LL(void);
int search_key(struct LinkedList *LL, char key);
char search_index(struct LinkedList *LL, int idx);
int insert_unordered(struct LinkedList *LL, char key);
int insert_ordered(struct LinkedList *LL, char key);
int remove_head(struct LinkedList *LL);
int remove_node(struct LinkedList *LL, char key);
void reverse(struct LinkedList *LL);
void printLL(struct LinkedList *LL);
void free_LL(struct LinkedList *LL);
void clear_string(char string[256]);


















