#include "huffman.h"

int main(int argc, char *argv[])
{
  int *ht = calloc(256, sizeof(int));
  char **codes = (char **)calloc(256, sizeof(char *));
  char *code = (char *)calloc(256, sizeof(char));
  char *in_path = "t1";
  int size;
  struct node *nodes;
  struct LinkedList LL;

  ht = getwords(in_path);
  size = count_nodes(ht);
  nodes = ht_elements(ht, size);
  LL = create_LL(nodes, size);
  create_tree(&LL);
  encode(LL.head, code, codes, 0);
  print_codes(codes);

  return 0;
}
