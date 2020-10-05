#ifndef LWPH
#define LWPH
#include <sys/types.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#if defined(__x86_64)
#include "fp.h"
typedef struct __attribute__ ((aligned(16))) __attribute__ ((packed))
registers {
  unsigned long rax;            /* the sixteen architecturally-visible regs. */
  unsigned long rbx;
  unsigned long rcx;
  unsigned long rdx;
  unsigned long rsi;
  unsigned long rdi;
  unsigned long rbp;
  unsigned long rsp;
  unsigned long r8;
  unsigned long r9;
  unsigned long r10;
  unsigned long r11;
  unsigned long r12;
  unsigned long r13;
  unsigned long r14;
  unsigned long r15;
  struct fxsave fxsave;   /* space to save floating point state */
} rfile;
#else
  #error "This only works on x86_64 for now"
#endif

typedef unsigned long tid_t;
#define NO_THREAD 0             /* an always invalid thread id */

typedef struct threadinfo_st *thread;
typedef struct threadinfo_st {
  tid_t         tid;            /* lightweight process id  */
  unsigned long *stack;         /* Base of allocated stack */
  size_t        stacksize;      /* Size of allocated stack */
  rfile         state;          /* saved registers         */
  thread        lib_one;        /* Two pointers reserved   */
  thread        lib_two;        /* for use by the library  */
  thread        sched_one;      /* Two more for            */
  thread        sched_two;      /* schedulers to use       */
} context;

typedef void (*lwpfun)(void *); /* type for lwp function */

/* Tuple that describes a scheduler */
typedef struct scheduler {
  void   (*init)(void);            /* initialize any structures     */
  void   (*shutdown)(void);        /* tear down any structures      */
  void   (*admit)(thread new);     /* add a thread to the pool      */
  void   (*remove)(thread victim); /* remove a thread from the pool */
  thread (*next)(void);            /* select a thread to schedule   */
} *scheduler;

/* lwp functions */
tid_t lwp_create(lwpfun,void *,size_t);
void  lwp_exit(void);
tid_t lwp_gettid(void);
void  lwp_yield(void);
void  lwp_start(void);
void  lwp_stop(void);
void  lwp_set_scheduler(scheduler fun);
scheduler lwp_get_scheduler(void);
thread tid2thread(tid_t tid);

/* Macros for stack pointer manipulation:
 *
 *  GetSP(var)      Sets the given variable to the current value of the
 *                  stack pointer.
 *  SetSP(var)      Sets the stack pointer to the current value of the
 *                  given variable.
 */
#if defined (__x86_64)         /* X86 only code */
  #define BAILSIGNAL SIGSTKFLT
  #define GetSP(sp)  asm("movq  %%rsp,%0": "=r" (sp) : )
  #define SetSP(sp)  asm("movq  %0,%%rsp":           : "r" (sp)  )
#else /* END x86 only code */
  #error "This stack manipulation code can only be compiled on an x86"
#endif

#if defined (__APPLE__)
  #undef BAILSIGNAL
  #define BAILSIGNAL SIGABRT
#endif

/* prototypes for asm functions */
#define load_context(c) (swap_rfiles(NULL,c))
#define save_context(c) (swap_rfiles(c,NULL))
void swap_rfiles(rfile *, rfile *to);

extern scheduler RoundRobin;


#endif