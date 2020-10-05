#include "fw.h"

/* this function creates a struct word by taking an input string.
it keeps track of the frequency of the word from the input files */
struct word create_word(char *string)
{
    struct word w;
    w.string = (char *)calloc(strlen(string) + 1, sizeof(char));
    strcpy(w.string, string);
    w.frequency = 1;
    return w;
}

/* this function creates a hashtable data structure given the number
of times the program has rehashed. each time rehash is called, this
function will create a hashtable twice its previous size */
struct hashtable create_hashtable(int rehash_count)
{
  struct hashtable ht;
  ht.word_count = 0;
  ht.words = (struct word *)calloc(128 * rehash_count, sizeof(struct word));
  ht.length = 128 * rehash_count;
  return ht;
}

/* this function is used to determine where in the hashtable a string
will be placed. this function in particular finds that index by adding
all the ascii values of each char in the input string */
int hash(char *string)
{
  int i;
  int hash_val = 1;

  for(i = 0; i < strlen(string); i++)
  {
    hash_val += string[i];
  }

  return hash_val;
}

/* this function is used to resolve collisions in the hashtable by creating
a quadratic int value to be added to the hashed index of the string */
int quadratic_collision(int n)
{
  return n * n + 1;
}

/* this file takes an input string and places it in the input hashtable
based on the strings hash value. the hashtable handles collisions using
quadratic probing */
void insert(char *string, struct hashtable *ht)
{
  int index = hash(string);
  int attempts = 0;

  while(1)
  {
    index = index % (*ht).length;
    /* if the value at the index is empty insert the word */
    if((*ht).words[index].string == NULL)
    {
      (*ht).words[index] = create_word(string);
      (*ht).word_count++;
      break;
    }
    /* if the indexed word is the same, increment frequency */
    else if(strcmp((*ht).words[index].string, string) == 0)
    {
      (*ht).words[index].frequency++;
      break;
    }
    /* if there is a collision then perform quadratic probing */
    else
    {
      attempts++;
      index += quadratic_collision(attempts);
    }
  }
}

/* helper function used for rehashing when the load factor is too large*/
void reinsert(struct word w, struct hashtable ht)
{
  int index = hash(w.string);
  int attempts = 0;

  while(1)
  {
    index = index % ht.length;
    /* if the value at the index is empty insert the word */
    if(ht.words[index].string == NULL)
    {
      ht.words[index] = w;
      break;
    }
    /* if there is a collision then perform quadratic probing */
    else
    {
      attempts++;
      index += quadratic_collision(attempts);
    }
  }
}

/* this function takes an input hashtable and calculates the load
factor by dividing the number of words by the size of the structure */
float load_factor(struct hashtable ht)
{
  return (float)ht.word_count / (float)ht.length;
}

/* this function takes an input hashtable and rehashes it based on the
tables load factor. if the table exceeds a load factor of 0.7, it will
rehash and place the old hashed words into a new hashtable of twice
the size of the old hash table */
void rehash(struct hashtable *ht, int *rehash_count)
{
  int i;
  struct hashtable new_ht = create_hashtable(*rehash_count * 2);

  for(i = 0; i < (*ht).length; i++)
  {
    if((*ht).words[i].string != NULL)
    {
      reinsert((*ht).words[i], new_ht);
    }
  }
  *rehash_count *= 2;
  free((*ht).words);
  (*ht).words = new_ht.words;
  (*ht).length = new_ht.length;
}

/* checks the load factor and rehashes if it exceeds 0.7 */
void check_rehash(struct hashtable *ht, int *rehash_count)
{
  if(load_factor(*ht) > 0.7)
  {
    rehash(ht, rehash_count);
  }
}

/* this function takes an input hashtable and breaks it down so there are no
empty nodes in the output array of words */
struct word* elements_in_hashtable(struct hashtable ht)
{
  int i;
  int j = 0;
  struct word *temp;

  temp = (struct word *)calloc(ht.word_count, sizeof(struct word));
  for(i = 0; i < ht.length; i++)
  {
    if(ht.words[i].string != NULL)
    {
      temp[j] = ht.words[i];
      j++;
    }
  }

  return temp;
}

/* <0 word1 goes before word2
0  word1 is equivalent to word2
>0 word1 goes after word2 */
int frequency_cmp(struct word word1, struct word word2)
{
  return word2.frequency - word1.frequency;
}

/* <0 word1 goes before word2
0  word1 is equivalent to word2
>0 word1 goes after word2 */
int alphabetic_cmp(struct word word1, struct word word2)
{
  return strcmp(word1.string, word2.string);
}

/* compare function used to compare two words in qsort() function */
int comparator(const void *arg1, const void *arg2)
{
  struct word word1 = *(struct word *)arg1;
  struct word word2 = *(struct word *)arg2;
  int cmp_val = frequency_cmp(word1, word2);

  if(!cmp_val)
  {
    return alphabetic_cmp(word2, word1);
  }

  return cmp_val;
}

/* performs quicksort on an input list based on word frequency, then
based on reverse lexographic order
*/
void sort_array(struct word *words, int unique_words)
{
  qsort(words, unique_words, sizeof(struct word), comparator);
}

/* takes an input string and char and appens the char to the end of
the input string */
char * append_char(char *string, char c)
{
  char temp[2];
  string = (char *)realloc(string, (strlen(string) + 4) * sizeof(char));

  temp[0] = c;
  temp[1] = '\0';
  strcat(string, temp);
  return string;
}

/* takes an input hashtable pointer and prints its contents */
void print_ht(struct hashtable *ht)
{
  int i;

  printf("Size of Hashtable: %d\n", (*ht).length);
  for(i = 0; i < (*ht).length; i++)
  {
    printf("%d:\t%s", i, (*ht).words[i].string);
    printf("\tfrequency: %d\n", (*ht).words[i].frequency);
  }
}

/* populates a hashtable given the user input file and flags */
struct hashtable get_words(char *argv[], int start_index, int num_files)
{
  int i;
  char c;
  int rehash_count = 1;
  struct hashtable ht = create_hashtable(rehash_count);

  for(i = start_index; i < num_files; i++)
  {
    char *string = (char *)malloc(2 * sizeof(char));
    FILE *file_in = fopen(argv[i], "r");
    string[0] = '\0';

    while((c = getc(file_in)) != EOF)
    {
      if(isalpha(c))
      {
        string = append_char(string, tolower(c));
      }
      else if(string[0] != '\0')
      {
        insert(string, &ht);
        check_rehash(&ht, &rehash_count);
        string[0] = '\0';
      }
    }

    if(string[0] != '\0')
    {
      insert(string, &ht);
      check_rehash(&ht, &rehash_count);
    }
    fclose(file_in);
  }
  /*free(string);*/
  return ht;
}

/* checks and opens the user specified file */
int checkfiles(char *argv[], int start_index, int num_files)
{
  int i;
  FILE *file;
  int all_files_exist = 1;
  for(i = start_index; i < num_files; i++)
  {
    if((file = fopen(argv[i], "r")))
    {
      fclose(file);
    }
    else
    {
      all_files_exist = 0;
    }
  }

  return all_files_exist;
}

/* frees all dynamically allocated memory structures */
void free_nodes(struct word *words, struct hashtable ht, int size)
{
  int i;
  for(i = 0; i < size; i++)
  {
    free(words[i].string);
  }

  free(ht.words);
}

/* takes the input array of words and prints out the n most frequent words
where n is specified by the user */
void printwords(struct word *words, int num_print, int word_count)
{
  int i;
  int max = num_print;

  if(max > word_count)
  {
    max = word_count;
  }

  printf("The top %d words (out of %d) are:\n", num_print, word_count);
  for(i = 0; i < max; i++)
  {
    printf("%9d %s\n", words[i].frequency, words[i].string);
  }

}
