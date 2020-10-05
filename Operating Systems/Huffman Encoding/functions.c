#include "huffman.h"

struct node * create_node(int freq, unsigned char c)
{
  struct node * n = (struct node *)calloc(1, sizeof(struct node));
  n->frequency = freq;
  n->value = c;
  n->left = NULL;
  n->right = NULL;
  n->next = NULL;
  return n;
}

struct node * create_parent(int freq, struct node *left, struct node *right)
{
  struct node * n = (struct node *)calloc(1, sizeof(struct node));
  n->frequency = freq;
  n->left = left;
  n->right = right;
  n->next = NULL;
  return n;
}

/* counts the number of chars with nonzero frequencies in the hashtable
so ht_elements knows how much space to callco */
int count_nodes(int *ht)
{
  int i;
  int count = 0;
  for(i = 0; i < 256; i++)
  {
    if(ht[i])
    {
      count++;
    }
  }
  return count;
}

/* takes the input hashtable and creates an array of nodes */
struct node ** ht_elements(int *ht, int size)
{
  int i, j;
  struct node **nodes = (struct node **)calloc(size, sizeof(struct node *));
  j = 0;
  for(i = 0; i < 256; i++)
  {
    if(ht[i])
    {
      nodes[j] = create_node(ht[i], (unsigned char)i);
      j++;
    }
  }
  return nodes;
}

/*
<0: n1 comes before n2
=0: n1 = n2
>0: n1 comes after n2
n1 is always the node you are inserting
*/
int compare(struct node *n1, struct node *n2)
{
  int cmp_val = 0;

  if(n2 == NULL)
  {
    return -1;
  }
  cmp_val = n1->frequency - n2->frequency;
  if(cmp_val == 0) /* frequency tie */
  {
    if(n1->value != 0) /* n1 is not a parent node */
    {
      cmp_val = n1->value - n2->value;
    }
    else /* n1 is a parent node with no char value */
    {
      return -1;
    }
  }
  return cmp_val;
}

/* takes in a linked list and a node and inserts the node into the linked
list in order based on frequency then ascii value */
void insert(struct LinkedList *LL, struct node *new)
{
  struct node *cur = NULL;
  if(LL->head == NULL)
  {
    LL->head = new;
    new->next = NULL;
    return;
  }/*
  if(LL->head->frequency == 0)
  {
    free(LL->head);
    LL->head = new;
    new->next = NULL;
    return;
  }*/
  if(compare(new, LL->head) < 0)
  {
    new->next = LL->head;
    LL->head = new;
    return;
  }
  for(cur = LL->head; cur->next != NULL &&
      compare(new, cur->next) > 0; cur = cur->next);
  new->next = cur->next;
  cur->next = new;
}

/* takes in a linked list and removes and returns a pointer to the head node */
struct node * remove_head(struct LinkedList *LL)
{
  struct node *tmp = NULL;
  if(LL->head != NULL)
  {
    tmp = LL->head;
    LL->head = LL->head->next;
    /*
    if(LL->head->next == NULL)
    {
      LL->head = (struct node *)calloc(1, sizeof(struct node));
    }
    else
    {
      LL->head = LL->head->next;
    }*/
  }

  return tmp;
} /* HOW HANDLE FREEING HERE */

/* takes in an array of nodes and returns the head node of the linked
list after inserting all nodes from the input array */
struct LinkedList * create_LL(struct node **nodes, int size)
{
  int i;
  struct LinkedList *LL = (struct LinkedList *)calloc(1, sizeof(struct LinkedList));
  LL->head = NULL;
  /*LL.head = (struct node *)calloc(1, sizeof(struct node));*/
  for(i = 0; i < size; i++)
  {
    insert(LL, nodes[i]);
  }
  LL->node_count = size;
  return LL;
}

/* takes an input linked list and creates the corresponding huffman
tree by removing the two front nodes and inserting the resulting parent node
back into the linked list until there is only 1 node left in the linked list */
struct node * create_tree(struct LinkedList *LL)
{
  int i;
  int frequency;

  for(i = 0; i < LL->node_count - 1; i++)
  {
    struct node *parent;
    struct node *n1;
    struct node *n2;
    n1 = remove_head(LL);
    n2 = remove_head(LL);
    n1->next = NULL;
    n2->next = NULL;
    frequency = n1->frequency + n2->frequency;
    if(compare(n1, n2) < 0)
    {
      parent = create_parent(frequency, n1, n2);
    }
    else
    {
      parent = create_parent(frequency, n2, n1);
    }
    insert(LL, parent);
  }
  return LL->head;
}

/* returns a 1 if the node is a leaf and a 0 if the node is not a leaf */
int is_leaf(struct node *n)
{
  if(n->left == NULL && n->right == NULL)
  {
    return 1;
  }
  return 0;
}

/* recursively creates the huffman codes for each leaf node and places them
in a new table */
void encode(struct node *root, char *string, char **codes, int END_CHAR)
{
  if(root == NULL)
  {
    return;
  }
  if(root->left)
  {
    string[END_CHAR] = '0';
    encode(root->left, string, codes, END_CHAR + 1);
  }

  if(root->right)
  {
    string[END_CHAR] = '1';
    encode(root->right, string, codes, END_CHAR + 1);
  }

  if(is_leaf(root))
  {
    codes[((int)root->value)] = (char *)calloc(strlen(string) + 1, sizeof(char));
    strncpy(codes[((int)root->value)], string, END_CHAR);
  }
}

void free_nodes(struct node **nodes, int size)
{
  int i;
  for(i = 0; i < size; i++)
  {
    free(nodes[i]);
  }
  free(nodes);
}

void free_tree(struct node *root)
{
  if(root->left != NULL)
  {
    free_tree(root->left);
  }
  if(root->right != NULL)
  {
    free_tree(root->right);
  }

  free(root);
}

void free_codes(char **codes)
{
  int i;
  for(i = 0; i < 256; i++)
  {
    free(codes[i]);
  }
  free(codes);
}

void write_codes(int *ht, char **codes, char *in_file, char *out_file, int count)
{
  int i;
  unsigned char acc = 0;
  int shift = 0;
  unsigned char c;
  int file_out = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0600);
  int file_in = open(in_file , O_RDONLY);
  int bytes_read = 0;
  /* write header */
  write(file_out, &count, sizeof(int)); /* write header size */
  for(i = 0; i < 256; i++)
  {
    if(codes[i] != 0)
    {
      write(file_out, &i, sizeof(char)); /* write the char */
      write(file_out, &(ht[i]), sizeof(int)); /* write the frequency of char */
    }
  }
  /* write body */
  while((bytes_read = read(file_in, &c, sizeof(char))) != 0)
  {
    char *code;
    code = codes[(unsigned int)c];
    while(*code != '\0')
    {
      acc = acc << 1;
      shift++;

      if(*code == '1')
      {
        acc |= 1;
      }

      if(shift == 8)
      {
        write(file_out, &acc, sizeof(char));
        shift = 0;
        acc = 0;
      }
      code++;
    }
  }
  if(shift != 8 && shift != 0)
  {
    acc = acc << (8 - shift);
    write(file_out, &acc, sizeof(char));
  }
  close(file_out);
  close(file_in);
}

/* read chars from file and increment frequency of each chars
corresponding ascii index for each occurrence */
int * getwords(char *pathname)
{
  int *ht = (int *)calloc(256, sizeof(int));
  int file = open(pathname, O_RDONLY);
  unsigned char c;
  int bytes_read = 0;
  while((bytes_read = read(file, &c, sizeof(char))) != 0)
  {
    ht[((int)c)]++;
  }
  close(file);
  return ht;
}

int node_counter(char *pathname)
{
  int num_nodes;
  int file = open(pathname, O_RDONLY);
  read(file, &num_nodes, sizeof(int));
  close(file);
  return num_nodes;
}
struct node ** rebuild_nodes(char *pathname)
{
  int file = open(pathname, O_RDONLY);
  int num_nodes, i, freq;
  unsigned char c;
  struct node **nodes;
  read(file, &num_nodes, sizeof(int));
  nodes = (struct node**)calloc(num_nodes, sizeof(struct node *));
  for(i = 0; i < num_nodes; i++)
  {
    read(file, &c, sizeof(char));
    read(file, &freq, sizeof(int));
    nodes[i] = create_node(freq, c);
  }
  close(file);
  return nodes;
}

void rewrite(char *file_in, char *file_out, struct node *head)
{
  int in_file = open(file_in, O_RDONLY);
  int out_file = open(file_out, O_WRONLY | O_TRUNC | O_CREAT, 0600);
  int freq, num_nodes, i, bytes_read;
  int total_chars = 0;
  struct node *tmp = head;
  unsigned char c = 0;
  int shift = 0;

  if(head != NULL)
  {
    total_chars = head->frequency;
  }

  read(in_file, &num_nodes, sizeof(int));
  for(i = 0; i < num_nodes; i++)
  {
    read(in_file, &c, sizeof(char));
    read(in_file, &freq, sizeof(int));
  }
  if(num_nodes == 1)
  {
    for(i = 0; i < freq; i++)
    {
      write(out_file, &c, sizeof(char));
    }
    close(in_file);
    close(out_file);
    return;
  }

  while((bytes_read = read(in_file, &c, sizeof(char))) != 0)
  {
    while(shift != 8)
    {
      if(c & 128) /* check if the MSB is a 1 or a 0 */
      {
        tmp = tmp->right;
      }
      else
      {
        tmp = tmp->left;
      }
      if(is_leaf(tmp))
      {
        write(out_file, &(tmp->value), sizeof(char));
        tmp = head;
        total_chars -= 1;
        if(total_chars == 0)
        {
            goto done;
        }
      }
      c = c << 1;
      shift++;
    }
    shift = 0;
  }
  done:
  close(in_file);
  close(out_file);
}

void print_codes(char ** codes)
{
  int i;
  for(i = 0; i <256; i++)
  {
    if(codes[i] != NULL)
    {
      printf("index %d: %s\n", i, codes[i]);
    }
  }
}

void printLL(struct LinkedList *LL)
{
  struct node *cur = LL->head;
  while(cur != NULL)
  {
    printf("[%c %d] -> ", cur->value, cur->frequency);
    cur = cur->next;
  }
  printf("[NULL]\n");
}
