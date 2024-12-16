#include "ScreenElements.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

TPool_t* tpool;
scr_t scr;
bool end = false;
pthread_mutex_t screenMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t onScreenUpdate = PTHREAD_COND_INITIALIZER;
bool screenUpToDate = false;
bool updateBallScheduled = false;
bool updateRightBarScheduled = false, updateLeftBarScheduled = false;
bool updateBackgroundScheduled = false;
bar_t leftBar, rightBar;
ball_t ball;

void* ballMovement() {
  while (!end) {
    clock_t start = clock();
    if ((ball.y == scr.y - 1) || (ball.y == 1)) {
      ball.movver = !ball.movver;
    }
    pthread_mutex_lock(&screenMutex);
    while (!screenUpToDate) {
      pthread_cond_wait(&onScreenUpdate, &screenMutex);
    }
    if (ball.x >= scr.x - 3) {
      ball.movhor = !ball.movhor;
      ball.movver = rand() % 2;

      if (ball.y < rightBar.y - 1 || ball.y > rightBar.y + 1) {
        leftBar.points++;
        ball.x = scr.x / 2;
        ball.y = rand() % (scr.y / 2) + scr.y / 4;

        if (!updateBackgroundScheduled) {
          addWorkTPool(tpool, updateBackground, NULL);
          updateBackgroundScheduled = true;
        }
      }
    }
    if (ball.x <= 2) {
      ball.movhor = !ball.movhor;
      ball.movver = rand() % 2;
      if (ball.y < leftBar.y - 1 || ball.y > leftBar.y + 1) {
        rightBar.points++;
        ball.x = scr.x / 2;
        ball.y = rand() % (scr.y / 2) + scr.y / 4;

        if (!updateBackgroundScheduled) {
          addWorkTPool(tpool, updateBackground, NULL);
          updateBackgroundScheduled = true;
        }
      }
    }
    ball.x = ball.movhor ? ball.x + 1 : ball.x - 1;
    ball.y = ball.movver ? ball.y + 1 : ball.y - 1;
    screenUpToDate = false;
    if (!updateBallScheduled) {
      addWorkTPool(tpool, updateBall, NULL);
      updateBallScheduled = true;
    }
    pthread_mutex_unlock(&screenMutex);

    clock_t end = clock();
    if ((double)(end - start) / CLOCKS_PER_SEC <
        1.0 / BALL_MOVEMENTS_PER_SECOND) {
      usleep(1000000 / BALL_MOVEMENTS_PER_SECOND -
             1000000 * (double)(end - start) / CLOCKS_PER_SEC);
    }
  }

  return NULL;
}

void* refreshScreen() {
  while (!end) {
    clock_t start = clock();
    pthread_mutex_lock(&screenMutex);
    refresh();
    screenUpToDate = true;
    pthread_cond_broadcast(&onScreenUpdate);
    pthread_mutex_unlock(&screenMutex);
    clock_t end = clock();
    if ((double)(end - start) / CLOCKS_PER_SEC <
        1.0 / SCREEN_REFRESH_PER_SECOND) {
      usleep(1000000 / SCREEN_REFRESH_PER_SECOND +
             1000000 * (double)(end - start) / CLOCKS_PER_SEC);
    }
  }

  return NULL;
}

void* barMovementAndStop() {
  while (!end) {
    int ch = getch();
    switch (ch) {
      case KEY_DOWN:
        pthread_mutex_lock(&screenMutex);
        while (!screenUpToDate) {
          pthread_cond_wait(&onScreenUpdate, &screenMutex);
        }
        rightBar.y++;
        if (rightBar.y <= 1) rightBar.y = scr.y - 2;
        if (rightBar.y >= scr.y - 1) rightBar.y = 2;
        screenUpToDate = false;
        if (!updateRightBarScheduled) {
          addWorkTPool(tpool, updateBar, &rightBar);
          updateRightBarScheduled = true;
        }
        pthread_mutex_unlock(&screenMutex);
        break;
      case KEY_UP:
        pthread_mutex_lock(&screenMutex);
        while (!screenUpToDate) {
          pthread_cond_wait(&onScreenUpdate, &screenMutex);
        }
        rightBar.y--;
        if (rightBar.y <= 1) rightBar.y = scr.y - 2;
        if (rightBar.y >= scr.y - 1) rightBar.y = 2;
        screenUpToDate = false;
        if (!updateRightBarScheduled) {
          addWorkTPool(tpool, updateBar, &rightBar);
          updateRightBarScheduled = true;
        }
        pthread_mutex_unlock(&screenMutex);
        break;
      case 'w':
        pthread_mutex_lock(&screenMutex);
        while (!screenUpToDate) {
          pthread_cond_wait(&onScreenUpdate, &screenMutex);
        }
        leftBar.y--;
        if (leftBar.y <= 1) leftBar.y = scr.y - 2;
        if (leftBar.y >= scr.y - 1) leftBar.y = 2;
        screenUpToDate = false;
        if (!updateLeftBarScheduled) {
          addWorkTPool(tpool, updateBar, &leftBar);
          updateLeftBarScheduled = true;
        }
        pthread_mutex_unlock(&screenMutex);
        break;
      case 's':
        pthread_mutex_lock(&screenMutex);
        while (!screenUpToDate) {
          pthread_cond_wait(&onScreenUpdate, &screenMutex);
        }
        leftBar.y++;
        if (leftBar.y <= 1) leftBar.y = scr.y - 2;
        if (leftBar.y >= scr.y - 1) leftBar.y = 2;
        screenUpToDate = false;
        if (!updateLeftBarScheduled) {
          addWorkTPool(tpool, updateBar, &leftBar);
          updateLeftBarScheduled = true;
        }
        pthread_mutex_unlock(&screenMutex);
        break;
      case 0x1B:
        end = true;
        break;
      case ' ':
        pthread_mutex_lock(&screenMutex);
        getch();
        pthread_mutex_unlock(&screenMutex);
        break;
    }
  }

  return NULL;
}

void* updateBackground() {
  pthread_mutex_lock(&screenMutex);
  screenUpToDate = false;
  mvprintw(2, scr.x / 2 - 5, "%4i | %i", leftBar.points, rightBar.points);
  mvvline(0, scr.x / 2, ACS_VLINE, scr.y);
  updateBackgroundScheduled = false;
  pthread_mutex_unlock(&screenMutex);

  return NULL;
}

void* updateBar(void* args) {
  bar_t* bar = args;
  pthread_mutex_lock(&screenMutex);
  for (int i = 0; i < scr.y; i++) {
    mvaddch(i, bar->x, ' ');
  }

  attron(COLOR_PAIR(1));
  for (int i = -1; i < 2; i++) {
    mvaddch(bar->y + i, bar->x, ACS_VLINE);
  }
  attroff(COLOR_PAIR(1));
  if (bar == &leftBar) {
    updateLeftBarScheduled = false;
  } else {
    updateRightBarScheduled = false;
  }
  pthread_mutex_unlock(&screenMutex);

  return NULL;
}

void* updateBall() {
  static int lastY = -1, lastX = -1;

  pthread_mutex_lock(&screenMutex);
  if (lastY != -1 && lastX != -1) {
    mvaddch(lastY, lastX, ' ');
    if (lastX == scr.x / 2 ||
        (lastY == 2 && (lastX >= scr.x / 2 - 4 && lastX <= scr.x / 2 + 4))) {
      if (!updateBackgroundScheduled) {
        addWorkTPool(tpool, updateBackground, NULL);
        updateBackgroundScheduled = true;
      }
    }
  }

  attron(COLOR_PAIR(1));
  mvaddch(ball.y, ball.x, 'O');
  attroff(COLOR_PAIR(1));

  lastY = ball.y;
  lastX = ball.x;
  updateBallScheduled = false;
  pthread_mutex_unlock(&screenMutex);

  return NULL;
}