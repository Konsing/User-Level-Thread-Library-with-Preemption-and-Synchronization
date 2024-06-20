#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "queue.h"
#include "uthread.h"

typedef enum {
  UTHREAD_STATE_READY,
  UTHREAD_STATE_RUNNING,
  UTHREAD_STATE_BLOCKED,
  UTHREAD_STATE_ZOMBIE,
} uthread_state_t;

typedef struct uthread_tcb {
  uthread_ctx_t ctx;
  uthread_state_t state;
  void *stack;
} uthread_tcb;

uthread_tcb *curr_thd = NULL;
uthread_tcb *prev_thd = NULL;
uthread_tcb *idle_thd = NULL;
queue_t ready_q;
queue_t zombie_q;

struct uthread_tcb *uthread_current(void) {
  return curr_thd;
}

void uthread_yield(void) {
  prev_thd = curr_thd;
  queue_dequeue(ready_q, (void **)&curr_thd);
  prev_thd->state = UTHREAD_STATE_READY;
  queue_enqueue(ready_q, prev_thd);
  curr_thd->state = UTHREAD_STATE_RUNNING;
  uthread_ctx_switch(&(prev_thd->ctx), &(curr_thd->ctx));
}

void uthread_exit(void) {
  prev_thd = curr_thd;
  queue_dequeue(ready_q, (void **)&curr_thd);
  prev_thd->state = UTHREAD_STATE_ZOMBIE;
  queue_enqueue(zombie_q, prev_thd);
  curr_thd->state = UTHREAD_STATE_RUNNING;
  uthread_ctx_switch(&(prev_thd->ctx), &(curr_thd->ctx));
}

// Create new threads and enqueue them (with memory leak prevention)
int uthread_create(uthread_func_t func, void *arg) {
  uthread_tcb *new_thd = malloc(sizeof(uthread_tcb));
  if (!new_thd)
    return -1;
  // allocating for stack and error check
  new_thd->stack = uthread_ctx_alloc_stack();
  if (!new_thd->stack) {
    free(new_thd);
    return -1;
  }

  new_thd->state = UTHREAD_STATE_READY; // set thread to ready

  // Initialize ctx and prevent memory leaks
  if (uthread_ctx_init(&(new_thd->ctx), new_thd->stack, func, arg) == -1) {
    uthread_ctx_destroy_stack(new_thd->stack);
    free(new_thd);
    return -1;
  }

  queue_enqueue(ready_q, new_thd); // enqueue it

  return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg) {
  // this section is new
  preempt_start(preempt);
  if(preempt)
		preempt_disable();
  // this section is new
  
  // Initialize both the zombie and ready queues. Also, memory leak check
  zombie_q = queue_create();
  if (!zombie_q) {
    return -1;
  }
  ready_q = queue_create();
  if (!ready_q) {
    queue_destroy(zombie_q);
    return -1;
  }

  uthread_create(NULL, NULL); // Create a null thread
  // dequeue the IDLE thread and make idle_thd point to the IDLE thread
  queue_dequeue(ready_q, (void **)&idle_thd);
                                     
  curr_thd = idle_thd; // set the current thread to IDLE thread
  curr_thd->state = UTHREAD_STATE_RUNNING;
  prev_thd = curr_thd; // set previous thread to running as well

  // this is new
  preempt_enable();

  uthread_create(func, arg);

  uthread_tcb *zombie_thd = NULL;

  while (1) {
    // check for and delete zombie threads
    while (queue_length(zombie_q) != 0) {
      queue_dequeue(zombie_q, (void **)&zombie_thd);
      uthread_ctx_destroy_stack(zombie_thd->stack);
      free(zombie_thd);
    }
    // while the ready_queue is not empty, yield
    if (queue_length(ready_q) != 0) {
      uthread_yield();
    } else {
      break;
    }
  }

  preempt_disable(); //***
  queue_destroy(ready_q);
  queue_destroy(zombie_q);
  uthread_ctx_destroy_stack(idle_thd->stack);
  free(idle_thd);

  return 0;
}

void uthread_block(void) {
  prev_thd = curr_thd;
  prev_thd->state = UTHREAD_STATE_BLOCKED;
  queue_dequeue(ready_q, (void **)&curr_thd);
  curr_thd->state = UTHREAD_STATE_RUNNING;
  uthread_ctx_switch(&(prev_thd->ctx), &(curr_thd->ctx));
}

void uthread_unblock(struct uthread_tcb *uthread) {
  uthread->state = UTHREAD_STATE_READY;
  queue_enqueue(ready_q, uthread);
}