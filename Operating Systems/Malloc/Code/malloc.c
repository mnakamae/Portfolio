#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define SBRK_SIZE 65536 /* normal sbrk size 2 ^ 16 */
#define big_SBRK 33554432  /* larger sbrk size 2^ 25 */
#define ALIGN 16
/* GLOBAL VARIABLES */
void *head = NULL; /* keeps track of first node in linked list */
int DEBUG_MALLOC = 0; /* set this on to turn on debug comments */  

/* header structure for the linked list applied to the heap */
typedef struct chonk {
   size_t size;
   char freed;
   struct chonk *next, *prev;
}chonk;

void free(void *ptr){
   chonk *cur;
   
   /* debugging output */
   if(DEBUG_MALLOC){
      printf("MALLOC: free(%p)\n", ptr);
   }
   
   /* if input is null, don't need to do any freeing */
   if(ptr == NULL){
      return;
   }
   
   /* DEBUGGING NOTE: not sure why this couldn't find internal ptrs */
   /* find the header of the data block the pointer belongs to */
   cur = (chonk *)head;
   while(cur->next != NULL && !(ptr > (void *)cur
         && ptr < (void *)(cur->next))){
      cur = cur->next;
   } 
   /* set the current header as a freed block */
   cur->freed = 0;

   /* combining adjacent free nodes */
   /* case where [FREE] -> [CUR] -> [NEXT]*/
   if(cur->prev != NULL && cur->prev->freed == 0){
      cur->prev->size = cur->prev->size + cur->size + 2 * sizeof(chonk);
      if(cur->next != NULL){
         cur->next->prev = cur->prev;
      }
      cur->prev->next = cur->next;
   }
   /* case where [PREV] -> [CUR] -> [FREE] */
   if(cur->next != NULL && cur->next->freed == 0){
      cur->size = cur->size + cur->next->size + 2 * sizeof(chonk);
      cur->next = cur->next->next;
   }
}


void *malloc(size_t size){
   int malloc_size;
   chonk *tmp, *prev = NULL;
   
   /* debugging output */
   if(DEBUG_MALLOC){
      printf("MALLOC: malloc(%d)\n", (int)size);
   }

   if(size <= 0){
      return NULL;
   }
   /* first call to malloc; instantiate heap */
   if(!head){
      /* check how big the first alloc call is */
      if(SBRK_SIZE < size){
         malloc_size = big_SBRK;
      }
      else{
         malloc_size = SBRK_SIZE;
      }
      head = sbrk(malloc_size);

      /* create the header containing the space created */
      chonk *first = (chonk *)head;
      first->freed = 1;

      /* note (16 - size % 16) comes from data alignment so that chunks
       * of data are carved out in factors of 16 */
      first->size = size + (ALIGN - size % ALIGN);
      first->prev = prev;

      /* create a second header containing the leftover memory space */
      chonk *second = (chonk *)((uintptr_t)head + 2 * sizeof(chonk)
            + size + (ALIGN - size % ALIGN));
      second->size = malloc_size - (size + (ALIGN - size % ALIGN)
            + 2 * 2 * sizeof(chonk));
      second->freed = 0;
      second->prev = first;
      second->next = NULL;
      first->next = second;

      /* return data space to user */
      return (void *)((uintptr_t)head + 2 * sizeof(chonk));
   }
   
   /* any following calls to malloc */
   tmp = (chonk *)head;

   /* find a spot to instert into the heap */
   while(tmp->next && ((uintptr_t)tmp->size <= size + (ALIGN - size % ALIGN)
            + 2 * sizeof(chonk) && tmp->freed)){
      tmp = tmp->next;
   }
   prev = tmp->prev;
   
   /* if no spot was found, request more memory */
   if(tmp->next == NULL && !(tmp->size <= size + (ALIGN - size % ALIGN) + 
            2 * sizeof(chonk) && tmp->freed == 1)){
      chonk *new = (chonk *)sbrk(SBRK_SIZE);
      new->size = size + (ALIGN - size % ALIGN);
      new->prev = tmp;
      tmp->next = new;
      new->freed = 1;
      
      /* create a node with all the leftover memory space */
      chonk *last = (chonk *)((uintptr_t)new + 2 * sizeof(chonk)
            + size + (ALIGN - size % ALIGN));
      last->size = SBRK_SIZE - size + (ALIGN - size % ALIGN)
         - 2 * 2 * sizeof(chonk);
      last->freed = 0;
      
      /* connect the sbrk */
      new->next = last;
      last->prev = new;
      last->next = NULL;
      return (void *)((uintptr_t)new + 2 * sizeof(chonk));
   }
   /* if a spot is found in the list */
   else if(tmp->size >= size + (ALIGN - size % ALIGN) + 2 * sizeof(chonk) 
         && tmp->freed == 0){
      tmp->freed = 1;
      chonk *last = (chonk *)((uintptr_t)tmp + 2 * sizeof(chonk)
            + size + (ALIGN - size % ALIGN));
      last->size = tmp->size - (size + (ALIGN - size % ALIGN)
            + 2 * sizeof(chonk));
      last->freed = 0;
      tmp->size = size + (ALIGN - size % ALIGN);
         
      last->prev = tmp;
      /* case where spot is at the end of the list */
      if(tmp->next == NULL){
         last->next = NULL;
      }
      /* case where spot is in the middle of the list */
      else{
         last->next = tmp->next;
      }
      tmp->next = last;
      return (void *)((uintptr_t)tmp + 2 * sizeof(chonk));
   }

   tmp->freed = 1;
   return (void *)((uintptr_t)tmp + 2 * sizeof(chonk));
}



void *calloc(size_t nmemb, size_t size){
   /* debugging output */
   if(DEBUG_MALLOC){
      printf("MALLOC: calloc(%d, %d)\n", (int)nmemb, (int)size);
   }
   
   /* create a memory space for the user */
   void *ptr = malloc(nmemb * size);
   /* clear the memory space and initialize everything to be 0 */
   memset(ptr, 0 , size + (ALIGN - size % ALIGN));
   return ptr;
}

void *realloc(void *ptr, size_t size){
   chonk *header, *next;
   void *tmp;
   int SPACE_REQ;
   
   /* debugging output */
   if(DEBUG_MALLOC){
      printf("MALLOC: realloc(%p, %d)\n", ptr, (int)size);
   }
   
   /* NULL pointer passed in as parameter */
   if(!ptr){
      return malloc(size);
   }
   /* if size 0 passed as parameter, free the pointer */
   if(size == 0){
      free(ptr);
      return NULL;
   }
   /* find which header needs to be resized */
   header = (chonk *)head;
   while(header->next != NULL && !(ptr > (void *)header
         && ptr < (void *)header->next)){
      header = header->next;
   } 
   
   /* DEBUGGING NOTES: THE LOGIC IN THESE IF STATEMENT BLOCKS WORK,
    * BUT THE PROGRAM DOES NOT GO INTO THE CORRECT BLOCKS FOR SOME REASON.
    * BECAUSE OF THIS I CAN PASS TEST CASES 42-47, BUT NOT ON A SINGLE
    * COMPILE */
   
   SPACE_REQ = size + (ALIGN - size % ALIGN) + 2 * sizeof(chonk);
   /* case where allocated space needs to be shrunk */
   if(header->size > SPACE_REQ){
      /* split the node into 2 nodes, one contains the leftover space */
      next = (chonk*)((uintptr_t)header + 2 * sizeof(chonk)
            + size + (ALIGN - size % ALIGN));
      next->size = header->size - (size + (ALIGN - size % ALIGN)
            + 2 * sizeof(chonk));
      header->size = size + (ALIGN - size % ALIGN);

      next->next = header->next;
      next->prev = header;
      header->next = next;

      /* free the leftover space so its available to the program */
      free((void *)next);
      return (chonk *)((uintptr_t)header + 2 * sizeof(chonk));
   }
   /* cases where allocated space needs to be expanded*/
   else if(header->size < SPACE_REQ){
      /* previous node and current node are both freed, so combine */
      if(header->prev != NULL && (header->prev)->freed == 0 
            && (header->size + (header->prev)->size) > SPACE_REQ){

         header->prev->size += header->size + SPACE_REQ;
         header->next->prev = header->prev;
         header->prev->next = header->next;
         memcpy((void *)((uintptr_t)header->prev + 2 * sizeof(chonk)),
               (void *)((uintptr_t)header + 2 * sizeof(chonk)),
               size + (ALIGN - size % ALIGN));

         return (void *)((uintptr_t)header->prev + 2 * sizeof(chonk));
      }
      /* current node and next node are both freed, so combine */
      else if(header->next != NULL && (header->next)->freed == 0
            && (header->size + (header->next)->size) > SPACE_REQ){
      
         header->size += header->next->size + SPACE_REQ;
         header->next = header->next->next;

         return (void *)((uintptr_t)header + 2 * sizeof(chonk));
      }
      /* case where allocated space needs to be expanded out of place*/
      else{
         tmp = malloc(size);
         memcpy((void *)(uintptr_t)tmp, (void *)header + 2 * sizeof(chonk),
               header->size);
         free(ptr);
         return tmp;
      }
   }
   
   /* case where size is the same and nothing needs to be done */
   return (void *)((uintptr_t)header + 2 * sizeof(chonk));
}
