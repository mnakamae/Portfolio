#include "huffman.h"

int main(int argc, char *argv[])
{
  int *ht = NULL;
  char **codes = (char **)calloc(256, sizeof(char *));
  char *code = (char *)calloc(256, sizeof(char));
  int size;
  struct node **nodes;
  struct LinkedList *LL;

  ht = getwords(argv[1]);
  size = count_nodes(ht);
  nodes = ht_elements(ht, size);
  LL = create_LL(nodes, size);
  create_tree(LL);

  encode(LL->head, code, codes, 0);
  write_codes(ht, codes, argv[1], argv[2], size);

  free(ht);
  free_codes(codes);
  free(code);
  free(nodes);
  if(LL->head != NULL)
  {
    free_tree(LL->head);
  }
  free(LL);
  return 0;
}
