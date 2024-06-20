# TLDR: Quick Overview Of The Program

This multithreading program consists of several files: queue.c, uthread.c,
sem.c, and preempt.c. These files work together to provide a multithreading
environment in which multiple threads can run concurrently.

queue.c provides a basic implementation of a queue data structure, which is
used to manage the various threads in the system.

uthread.c provides the implementation of the user-level threads that are used
in the multithreading environment. It uses a round-robin scheduling algorithm
to ensure that each thread gets a chance to run, and it provides functions for
creating, starting, stopping, and joining threads.

sem.c provides the implementation of semaphores, which are used to manage
access to shared resources. It provides functions for creating, destroying,
and manipulating semaphores.

preempt.c provides the implementation of preemption, which is the process of
temporarily interrupting a task being carried out by a thread, without
requiring its cooperation, with the goal of allowing other threads to execute.
It provides functions for starting and stopping the preemption timer.

Together, these files provide a framework for creating and managing threads in
a multithreading environment. Threads can use semaphores to manage access to
shared resources, and preemption ensures that all threads get a chance to
execute in a fair and timely manner.

# Thorough Overview of Program 
## Introduction

This is a multithreading program that implements user-level threads,
semaphores, and preemption. It defines a user-level thread (uthread) as a
lightweight process that shares the same virtual address space with other
threads. The program provides basic operations on uthreads such as creating,
joining, and yielding.

In addition, the program implements semaphores using uthreads to provide
synchronization among threads. The semaphores are implemented using a queue
of threads waiting to acquire a semaphore. The program provides basic
operations on semaphores such as initialization, waiting, and signaling.

Furthermore, the program implements preemption by defining a timer that
generates a signal at a specified frequency. When the signal is received, the
current thread is preempted, and another thread is scheduled to run.

The program uses a few C files, including uthread.c, queue.c, sem.c, and
preempt.c, to provide the above functionalities. The queue.c file provides a
queue data structure used in the implementation of semaphores. The preempt.c
file defines the preemption mechanism. The uthread.c file provides the
implementation of uthreads and their basic operations. The sem.c file provides
the implementation of semaphores and their basic operations.

Overall, this multithreading program provides a simple and efficient way to
create and manage user-level threads, and synchronize threads using semaphores.

## Program Structure

# queue.c

This is the implementation of a queue data structure in C. The queue data
structure is a data container that operates on a first-in, first-out (FIFO)
basis, meaning that the first item added to the queue is the first one to be 
removed.

The file includes a struct node and a struct queue. The node contains a void
pointer to data and a pointer to the next node in the queue. The queue contains
pointers to the front and rear nodes of the queue and the size of the queue.

The file includes functions to create, destroy, enqueue, dequeue, and delete
from the queue.

queue_create function creates a new queue by allocating memory for a queue
structure and initializing its fields.

queue_destroy function deallocates a queue by freeing its memory. However,
it returns -1 if the queue is not empty, as it cannot be destroyed when there
are still elements in the queue.

queue_enqueue function inserts a new element to the rear of the queue. The
function creates a new node and inserts it at the end of the queue, updating
the rear pointer.

queue_dequeue function removes an element from the front of the queue and
returns its data. The function sets the data pointer to the address of the
dequeued element, frees its memory, and updates the front pointer.

queue_delete function deletes an element from the queue. The function finds
the element with matching data, removes it from the queue, and updates the
links between the nodes.

All of the queue functions return -1 on failure, 0 on success, and require
a queue_t pointer as the first argument.

# uthread.c

This is a C implementation of user-level threads (or lightweight threads) using
the uthread library. The library provides functions for creating, blocking, and
unblocking threads. The implementation is based on a cooperative multitasking
model, which means that threads must voluntarily yield control to other threads,
instead of being interrupted by the operating system scheduler.

The library defines four states for a thread: READY, RUNNING, BLOCKED, and
ZOMBIE. A thread in the READY state is ready to run but is waiting for the CPU.
A thread in the RUNNING state is currently running. A thread in the BLOCKED
state is waiting for a resource or event to become available. A thread in the
ZOMBIE state has finished executing, and its resources (e.g., stack) are
waiting to be released.

The main data structure used by the library is the uthread_tcb (Thread Control
Block), which holds the context of the thread (registers, stack pointer, etc.),
its state, and a pointer to its stack. The library also uses two queues: the
ready queue, which holds threads in the READY state, and the zombie queue,
which holds threads in the ZOMBIE state.

The main functions provided by the library are:

uthread_create:  creates a new thread with the given function and argument and
                 adds it to the ready queue.
uthread_yield:   yields the CPU to another thread in the ready queue.
uthread_exit:    marks the current thread as a zombie and yields the CPU to
                 another thread.
uthread_block:   blocks the current thread and yields the CPU to another thread.
uthread_unblock: unblocks a blocked thread and adds it to the ready queue.

The library also provides two helper functions:

uthread_current: returns a pointer to the current thread's TCB.
uthread_run:     starts the scheduler and runs the threads in the ready queue
                 until there are no more threads left. It creates an idle
                 thread that runs when there are no other threads in the ready
                 queue.
                 
The implementation uses the ucontext library to implement the context switching
between threads. The preempt_start and preempt_enable functions are new to this
implementation and are not part of the uthread library. They are used to enable
and disable a timer-based preemption mechanism, which would interrupt threads
at regular intervals to prevent them from monopolizing the CPU.

# sem.c

This is a C implementation of semaphores using queues, which is commonly used
for synchronization in multithreaded programs.

This code implements a simple semaphore using a struct that contains
an unsigned long long count and a queue. The semaphore can be created
using the 'sem_create()' function, which allocates space for the semaphore,
initializes its count to a specified value, and creates a new queue to
hold waiting threads.

The 'sem_destroy()' function can be used to destroy the semaphore and free
the memory it uses. This function returns -1 if there are any threads
waiting on the semaphore.

The 'sem_down()' function is used to decrement the count of the semaphore.
If the count is zero, the calling thread is added to the queue of waiting
threads and blocked. When the semaphore is later incremented by another
thread using 'sem_up()', the first thread in the queue is unblocked and
allowed to proceed.

The 'sem_up()' function is used to increment the count of the semaphore. If
there are any threads waiting in the semaphore's queue, the first thread
in the queue is dequeued and unblocked using uthread_unblock(). The
sem_up() function returns -1 if there are no threads waiting on the
semaphore.

This implementation of semaphores is relatively simple and uses a single queue
to manage waiting threads. The uthread library is used to block and unblock
threads.

# preempt.c

The preempt.c file contains code for enabling and disabling preemption, as well
as setting up a timer to generate a preemption signal. The #define HZ 100
defines the frequency of preemption, which is set to 100 times per second.

Functions:

The sigset_t ss defines a signal set, struct itimerval timer defines a timer,
and struct sigaction sa defines an action to be taken on a signal.

The alarm_handler function is called whenever the timer signals the occurrence
of a preemption event. This function calls uthread_yield() to yield the CPU to
another thread and prints a message to stderr.

The preempt_disable function blocks preemption by calling sigprocmask with the
SIG_BLOCK argument and the ss signal set.

The preempt_enable function unblocks preemption by calling sigprocmask with the
SIG_UNBLOCK argument and the ss signal set.

The preempt_start function sets up the timer to generate a preemption signal if
preempt is true. It registers the alarm_handler function to handle the SIGVTALRM
signal generated by the timer. It also initializes the timer struct with the HZ
frequency, sets the timer to zero and sets it to send the SIGVTALRM signal when
the timer expires, and starts the timer by calling setitimer with the
ITIMER_VIRTUAL argument.

The preempt_stop function disables preemption and stops the timer by calling
preempt_disable and setitimer with the ITIMER_VIRTUAL argument, respectively.