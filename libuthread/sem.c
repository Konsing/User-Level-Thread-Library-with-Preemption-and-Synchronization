#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

typedef unsigned long long usize;

typedef struct semaphore{
	usize sem_count;
	queue_t sem_queue;
} semaphore;

sem_t sem_create(size_t count){
	sem_t new_sem = malloc(sizeof(semaphore));
	
	if (!new_sem){
		return NULL;
  	}

	new_sem->sem_count = count;
	new_sem->sem_queue = queue_create();

	if (!new_sem->sem_queue){
		free(new_sem);
		return NULL;
	}

	return new_sem;
}

int sem_destroy(sem_t sem){
	if (!sem){
		return -1;
  	}

	if (queue_length(sem->sem_queue) > 0){
		return -1;
  	}

	queue_destroy(sem->sem_queue);
	free(sem);

	return 0;
}

int sem_down(sem_t sem){
	if (!sem){
		return -1;
  	}

	if (sem->sem_count == 0){
		struct uthread_tcb *curr_thd = uthread_current();
		queue_enqueue(sem->sem_queue, curr_thd);

		uthread_block();
	}

	sem->sem_count--;
	return 0;
}

int sem_up(sem_t sem){
	if (!sem){
		return -1;
  	}

	sem->sem_count++;

	if (queue_length(sem->sem_queue) > 0){
		struct uthread_tcb *first_ready = NULL;

		if (queue_dequeue(sem->sem_queue, (void **)&first_ready) == -1){
			return -1;
    	}

		uthread_unblock(first_ready);
	}

	return 0;
}