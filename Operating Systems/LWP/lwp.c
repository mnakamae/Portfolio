#include "lwp.h"
#include "stddef.h"
#include "stdio.h"
#include "scheduler.h"
#include "stdlib.h"

#define SPOFFSET 2 /* offset used to set SP on stack frame */
#define BPOFFSET 3 /* offset used to set BP on stack frame */
#define FUNOFFSET 2 /* offset used to set func on stack frame */
#define EXITOFFSET 1 /* offset used to set lwp_exit on stack frame */
#define NOPROC 1 /* used to check if first process created yet */
#define SMALLSTACK 2048 /* smallest size for an allocated space */  
#define ALIGN 8 /*used to modulo the stack so it divides evenly*/

static int counter = NOPROC; /* counter to keep track of tid's */
static thread current; /* current running thread */
static thread head; /* head of singly linked list */
static rfile sys_status; /* holds original save state for registers */
static unsigned long *island = NULL; /* pointer to safe spot for lwp_exit */

/* singly linked list insert */
void ll_insert(thread new)
{
   thread tmp = head;
   if (head == NULL)
   {
      head = new;
      new -> lib_two = NULL;
      return;
   }
   while (tmp -> lib_two != NULL)
   {
      tmp = tmp -> lib_two;
   }
   tmp -> lib_two = new;
}

/* singly linked list remove*/
void ll_remove(thread carl)
{
   thread tmp = head;
   if (carl -> tid == head -> tid) 
   {
      head = head -> lib_two;
      return;
   }
   while((tmp -> lib_two != NULL ) && (carl -> tid != tmp -> lib_two -> tid))
   {
      tmp = tmp -> lib_two;
   }
   if (carl -> tid == tmp -> lib_two -> tid) 
   {
      tmp -> lib_two  = tmp -> lib_two -> lib_two;
   }
}

/* instantiates stack frame for new thread and adds thread to
 * the current scheduler */
tid_t lwp_create(lwpfun fun,void *arg, size_t stack_size)
{
   unsigned long *stack;
   struct threadinfo_st *t;
   /*check for stack sizes that are too small*/
   if(stack_size < SMALLSTACK)
   {
      stack_size = SMALLSTACK;
   }
   /*making sure the stack size is divible by 8*/
   if(stack_size %ALIGN)
   {
      stack_size += ALIGN - ( stack_size % ALIGN);
   }

   stack = (unsigned long *)malloc(stack_size );
   t = (struct threadinfo_st *) malloc(sizeof(context));
   if (island == NULL) /* create safe space for lwp_exit to use */
   {
      island = (unsigned long *)malloc(stack_size);
   }

   /* error check malloc calls */
   if (stack == NULL || t == NULL || island == NULL)
   {
      fprintf(stderr, "bad malloc :(\n");
      perror(NULL);
   }

   /* set up context attributes for new thread */
   t -> tid = counter++;
   t -> stack = stack;
   t -> stacksize = stack_size/sizeof(long);
   
   /* initialize stack frame*/
   t -> state.fxsave = FPU_INIT;
   t -> state.rdi = (unsigned long)arg;

   t -> stack[stack_size/sizeof(long) - EXITOFFSET] = (unsigned long) lwp_exit;
   t -> stack[stack_size/sizeof(long) - FUNOFFSET] = (unsigned long) fun;
   t -> state.rsp = (unsigned long) (stack +
      (stack_size/sizeof(long)  - SPOFFSET));
   t -> state.rbp = (unsigned long) (stack +
      (stack_size/sizeof(long) - BPOFFSET));

   t -> lib_two = NULL;
   t -> lib_one = NULL;

   t -> sched_one = NULL;
   t -> sched_two = NULL;

   /* admit new thread to scheduler */
   ll_insert(t);
   sched -> admit(t);

   return t -> tid;
}

/* helper function to free memory */
void Really_exit(thread trash)
{   
   if (trash->stack)
   {
      free(trash -> stack);
   }
   free(trash);
   if (current == NULL) /* for last thread, free safe space */
   {
      load_context(&sys_status);
      free(island);
      island = NULL;
   }
   else
   {
      load_context(&(current -> state));
   }

}

/* terminates current thread and frees its resources, then restores
 * the original system thread */
void  lwp_exit(void)
{
   thread tmp = current;
   if(tmp == NULL){
      lwp_stop();
   }
   ll_remove(tmp);
   sched -> remove(tmp);
   current = sched->next();
   SetSP((unsigned long *) island+tmp -> stacksize);
   Really_exit(tmp);
}

/* returns tid of the currently running process */
tid_t lwp_gettid(void)
{
   return (current == NULL)? NO_THREAD : current -> tid ;
}

/* yield control to next thread, save current context, then restore
 * next threads context */
void  lwp_yield(void)
{
   thread tmp = current;
   current = sched -> next();
   if (current == NULL)
   {
      load_context(&sys_status);
   }
   else
   {
      swap_rfiles(&(tmp->state),&(current -> state));
   }

}

/* saves original context and picks thread to run */
void  lwp_start(void)
{
   if (counter == NOPROC) /* NULL check */
   {
      return;
   }

   current = sched -> next();

   if (current == NULL)
   {
      load_context(&sys_status);
   }
   else 
   {
      swap_rfiles(&sys_status, &(current -> state));
   }
}

/* restores original SP, returns to that context */
void  lwp_stop(void)
{
   if (current)
   {
      save_context(&(current -> state));
      current = NULL;
   }
   load_context(&sys_status);
}

/* transfers threads from previous scheduler to new scheduler and
 * uses the new scheduler until changed again */
void  lwp_set_scheduler(scheduler fun)
{
   thread tmp;
   if (fun -> init) /* check/run for new scheduler init func */
   {
      fun -> init();
   }

   /* move contents from old scheduler to new scheduler */
   tmp = sched -> next();
   while(tmp)
   {
      sched ->remove(tmp);
      fun -> admit(tmp);
      tmp = sched -> next();
   }
   if (sched -> shutdown) /* check/run for old scheduler shut down func */
   {
      sched -> shutdown();
   }

  sched = fun; 
}

/* returns current scheduler */
scheduler lwp_get_scheduler(void)
{
   return sched;
}

/* returns thread given its tid, returns NULL if tid not found */
thread tid2thread(tid_t tid)
{
   thread tmp = head;
   while(tmp != NULL){
      if (tmp -> tid == tid)
      {
         return tmp;
      }
      tmp = tmp -> lib_two;
   }
   return NULL;
}