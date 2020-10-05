#include "lwp.h"
#include "scheduler.h"
#include "stddef.h"

static thread scurr = NULL; /* the next thread to be run by sched */

/* instantiate extern scheduler for lwp */
struct scheduler rr_publish = {NULL, NULL, rr_admit, rr_remove, rr_next};
scheduler sched = &rr_publish;

void rr_admit(thread new)     /* add a thread to the pool  at the end     */
{
   if (scurr == NULL) /* add first thread into scheduler */
   {
      scurr = new;
      scurr -> sched_one = scurr;
      scurr -> sched_two = scurr;
   }
   /* link ends of doubly linked list */
   new -> sched_two = scurr;
   new -> sched_one = scurr -> sched_one;
   scurr -> sched_one -> sched_two = new;
   scurr -> sched_one = new;
}

void rr_remove(thread victim) /* remove a thread from the pool */
{
   thread tmp;
   
   /* case for 0 or 1 threads in the doubly linked list */
   if (scurr == NULL || ((victim ->tid == scurr ->tid) &&
      (scurr -> tid == scurr -> sched_two -> tid)))
   {
      scurr = NULL;
      return;
   }
   tmp = scurr -> sched_two;

   /* find the victim in the doubly linked list */
   while((tmp -> tid != scurr -> tid ) && (victim -> tid != tmp -> tid))
   {
      tmp = tmp -> sched_two;
   }

   if (victim -> tid == tmp -> tid) /* found victim in linked list */
   {
      if (tmp -> tid == tmp -> sched_two -> tid)
      {
         scurr = NULL;
         return;
      }
      tmp -> sched_one -> sched_two = tmp -> sched_two;
      tmp -> sched_two -> sched_one = tmp -> sched_one;
   }
}

thread rr_next(void)            /* select a thread to schedule   */
{
   thread oldhead = scurr;
   if (scurr == NULL)         /* null check the head of round robin */
   {
      return NO_THREAD;
   }
   scurr = scurr -> sched_two; /* move scheduler along return next in line */
   return oldhead;
}