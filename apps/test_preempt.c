#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

void thread2(void *arg) {
  (void)arg;

  printf("thread2\n");
  int i = 0;
  while (1) {
    i++;
    if (i % 1000000 == 0)
      printf("2:%d\n", i);
    if (i == 10e7)
      break;
  }
  printf("thread2 headed out\n");
}

void thread1(void *arg) {
  (void)arg;

  uthread_create(thread2, NULL);
  printf("thread1\n");
  int i = 0;
  while (1) {
    i++;
    if (i % 1000000 == 0)
      printf("1:%d\n", i);
    if (i == 10e7)
      break;
  }
  printf("thread1 headed out\n");
}

int main(void) {
  uthread_run(true, thread1, NULL);
  return 0;
}