#ifndef PONG_TPOOL_H_
#define PONG_TPOOL_H_

#include <stdbool.h>
#include <stddef.h>
#include <pthread.h>

struct tpool_work {
  void* (*func)(void *);
  void *arg;
  struct tpool_work *next;
};
typedef struct tpool_work tpool_work_t;

struct TPool {
  tpool_work_t *work_first;
  tpool_work_t *work_last;
  pthread_mutex_t work_mutex;
  pthread_cond_t work_cond;
  pthread_cond_t working_cond;
  size_t working_cnt;
  size_t thread_cnt;
  bool stop;
};
typedef struct TPool TPool_t;

TPool_t *createTPool(size_t num);
void deleteTPool(TPool_t *tm);

bool addWorkTPool(TPool_t *tm, void* (*func)(void *), void *arg);
void waitTPool(TPool_t *tm);

#endif /* PONG_TPOOL_H_ */
