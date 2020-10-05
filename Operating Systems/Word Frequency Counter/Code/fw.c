#include "fw.h"

int main(int argc, char *argv[])
{
  int start_index;
  int valid = 0;
  struct word *unique;
  struct hashtable ht;
  int num_print = 10;

  if(argv[1][0] == '-')
  {
    if(argv[1][1] == 'n' && atoi(argv[2]) != 0)
    {
      start_index = 3;
      num_print = atoi(argv[2]);
      valid = 1;
    }
    else
    {
      valid = 0;
    }
  }
  else
  {
    valid = 1;
    start_index = 1;
  }

  if(!valid)
  {
    printf("usage: fw [-n num] [ file1 [file2 ...] ]\n");
  }
  else if(checkfiles(argv, start_index, argc))
  {
    ht = get_words(argv, start_index, argc);
    unique = elements_in_hashtable(ht);
    sort_array(unique, ht.word_count);
    printwords(unique, num_print, ht.word_count);
    free_nodes(unique, ht, ht.word_count);
  }
  /* unable to open files */
  else
  {
    printf("nonexistant: No such file or directory\n");
  }

  return 0;
}
