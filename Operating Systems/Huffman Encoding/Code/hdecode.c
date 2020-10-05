#include "huffman.h"

int main(int argc, char *argv[])
{
  int size;
  char **codes = (char **)calloc(256, sizeof(char *));
  char *code = (char *)calloc(256, sizeof(char));
  struct node **nodes;
  struct LinkedList *LL;

  /* DECODING */
  nodes = rebuild_nodes(argv[1]);
  size = node_counter(argv[1]);
  LL = create_LL(nodes, size);
  create_tree(LL);

  encode(LL->head, code, codes, 0);
  rewrite(argv[1], argv[2], LL->head);

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
