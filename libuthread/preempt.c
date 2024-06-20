#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

sigset_t ss;
struct itimerval timer;
struct sigaction sa;

void alarm_handler(int signum){
  uthread_yield();
  fprintf(stderr, "it is me the chain breaker |||stupid unused signum: %d\n", signum);
}

void preempt_disable(void){
  sigprocmask(SIG_BLOCK, &ss, NULL);
}

void preempt_enable(void){
  sigprocmask(SIG_UNBLOCK, &ss, NULL);
}

void preempt_start(bool preempt){
  if(preempt){
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGVTALRM, &sa, NULL);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = HZ*100;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = HZ*100;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);
  }
}

void preempt_stop(void){
  preempt_disable();
  setitimer(ITIMER_VIRTUAL, &timer, NULL);
}