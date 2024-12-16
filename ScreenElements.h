#ifndef PONG_SCREEN_ELEMENTS_H_
#define PONG_SCREEN_ELEMENTS_H_

#include <stdbool.h>
#include <pthread.h>

#include "ThreadPool.h"

#define BALL_MOVEMENTS_PER_SECOND 20
#define SCREEN_REFRESH_PER_SECOND 60

typedef struct scr {
  int x, y;
} scr_t;
typedef struct ball {
  int x, y;
  bool movhor, movver;
} ball_t;
typedef struct bar {
  int x, y, points;
} bar_t;

extern TPool_t* tpool;
extern scr_t scr;
extern bool end;
extern pthread_mutex_t screenMutex;
extern bar_t leftBar, rightBar;
extern ball_t ball;

void* updateBackground();
void* updateBar(void* args);
void* updateBall();

void* ballMovement();
void* refreshScreen();
void* barMovementAndStop();


#endif