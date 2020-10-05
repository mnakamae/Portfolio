#ifndef SCHEDULER
#define SCHEDULER
#include <sys/types.h>
#include <stdlib.h>

void rr_admit(thread new);     /* add a thread to the pool */

void rr_remove(thread victim); /* remove a thread from the pool */

thread rr_next(void);            /* select a thread to schedule   */


struct scheduler rr_publish;
extern scheduler sched;

#endif