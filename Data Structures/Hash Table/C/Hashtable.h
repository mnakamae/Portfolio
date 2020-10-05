#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/* STRUCTURES */
struct node{
	int value;
	char *key;
};

struct hashtable{
	struct node **values;
	int ht_size;
	int num_nodes;
};

