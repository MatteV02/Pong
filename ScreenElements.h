#ifndef PONG_SCREEN_ELEMENTS_H_
#define PONG_SCREEN_ELEMENTS_H_

#include <stdbool.h>
#include <pthread.h>

#include "ThreadPool.h"

#define BALL_MOVEMENTS_PER_SECOND 20  // number of ball movements per second
#define SCREEN_REFRESH_PER_SECOND 60  // screen refresh rate

/**
 * @brief Describes the screen region
 */
typedef struct scr {
  int x, y;
} scr_t;

/**
 * @brief Describes the ball and its direction
 */
typedef struct ball {
  int x, y;

  bool movhor,  // when true the ball is moving rightwards, when false the ball is moving leftwards
   movver;      // when true the ball is moving downwards, when false the ball is moving upwards
} ball_t;

/**
 * @brief Describes the bars contains the point of each player
 */
typedef struct bar {
  int x, y, points;
} bar_t;

/**
 * @brief Thread pool for temporary tasks
 */
extern TPool_t* tpool;

/**
 * @brief Screen region
 */
extern scr_t scr;

/**
 * @brief When true the game is over, when false the game goes on
 */
extern bool end;

/**
 * @brief Mutex which controls the access to the synchronization monitor
 */
extern pthread_mutex_t screenMutex;

/**
 * @brief Player bars
 */
extern bar_t leftBar, rightBar;

/**
 * @brief The ball
 */
extern ball_t ball;

/**
 * @brief Temporary thread for updating the background graphics
 * @return NULL 
 */
void* updateBackground();

/**
 * @brief Temporary thread for updating the bar graphic
 * @param args The bar structure to update
 * @return NULL
 */
void* updateBar(void* args);

/**
 * @brief Temporary thread for updating the ball graphics
 * @return NULL
 */
void* updateBall();


/**
 * @brief Permanent thread for updating the ball position over time
 * @return NULL
 */
void* ballMovement();

/**
 * @brief Permanent thread for refreshing the screen
 * @return NULL
 */
void* refreshScreen();

/**
 * @brief Permanent thread for user input management, it moves the bars and therminates the game
 * @return NULL
 */
void* barMovementAndStop();


#endif