#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

/* STRUCT TYPES */
struct word
{
  char *string;
  int frequency;
};

struct hashtable
{
  struct word *words;
  int length;
  int word_count;
};

/* FUNCTION PROTOTYPES */
void print_ht(struct hashtable *ht);
void free_nodes(struct word *words, struct hashtable ht, int size);
int checkfiles(char *argv[], int start_index, int num_files);
struct word create_word(char *string);
struct hashtable create_hashtable(int rehash_count);
int hash(char *string);
int quadratic_collision(int n);
void insert(char *string, struct hashtable *ht);
float load_factor(struct hashtable ht);
void rehash(struct hashtable *ht, int *rehash_count);
void check_rehash(struct hashtable *ht, int *rehash_count);
struct word* elements_in_hashtable(struct hashtable ht);
int frequency_cmp(struct word word1, struct word word2);
int alphabetic_cmp(struct word word1, struct word word2);
int comparator(const void *arg1, const void *arg2);
void sort_array(struct word *words, int unique_words);
char * append_char(char *string, char c);
struct hashtable get_words(char *argv[], int start_index, int num_files);
void printwords(struct word *words, int num_print, int word_count);
