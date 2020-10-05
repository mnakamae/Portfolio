#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* STRUCTURES */
/* LESSER KEYS ARE STORED IN THE LEFT NODE
GREATER KEYS ARE STORED IN THE RIGHT NODE */
struct node{
	int key;
	struct node *left;
	struct node *right;
};

struct bst{
  struct node *root;
};


/* FUNCTION PROTOTYPES */
struct node * create_node(int key);
struct bst * create_bst(void);
int contains_helper(struct node *parent, int key);
int contains(struct bst *tree, int key);
int insert_helper(struct node **parent, int key);
int insert(struct bst *tree, int key);
int follow_path(struct bst *tree, char path[256]);
void clear_string(char string[256]);



























