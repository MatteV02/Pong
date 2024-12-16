#include <ncurses.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "ScreenElements.h"
#include "ThreadPool.h"

int main() {
  srand(time(NULL));
  tpool = createTPool(4);
  initscr();
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  keypad(stdscr, true);
  noecho();
  curs_set(0);
  getmaxyx(stdscr, scr.y, scr.x);
  rightBar.x = scr.x - 2;
  rightBar.y = scr.y / 2;
  rightBar.points = 0;
  leftBar.x = 1;
  leftBar.y = scr.y / 2;
  leftBar.points = 0;

  ball.x = scr.x / 2;
  ball.y = scr.y / 2;
  ball.movhor = false;
  ball.movver = false;
  mvprintw(1, 0,
           "\t              _____                   _______                   "
           "_____                    _____          \n"
           "\t             /\\    \\                 /::\\    \\               "
           "  /\\    \\                  /\\    \\         \n"
           "\t            /::\\    \\               /::::\\    \\              "
           " /::\\____\\                /::\\    \\        \n"
           "\t           /::::\\    \\             /::::::\\    \\             "
           "/::::|   |               /::::\\    \\       \n"
           "\t          /::::::\\    \\           /::::::::\\    \\           "
           "/:::::|   |              /::::::\\    \\      \n"
           "\t         /:::/\\:::\\    \\         /:::/~~\\:::\\    \\         "
           "/::::::|   |             /:::/\\:::\\    \\     \n"
           "\t        /:::/__\\:::\\    \\       /:::/    \\:::\\    \\       "
           "/:::/|::|   |            /:::/  \\:::\\    \\    \n"
           "\t       /::::\\   \\:::\\    \\     /:::/    / \\:::\\    \\     "
           "/:::/ |::|   |           /:::/    \\:::\\    \\   \n"
           "\t      /::::::\\   \\:::\\    \\   /:::/____/   \\:::\\____\\   "
           "/:::/  |::|   | _____    /:::/    / \\:::\\    \\  \n"
           "\t     /:::/\\:::\\   \\:::\\____\\ |:::|    |     |:::|    | "
           "/:::/   |::|   |/\\    \\  /:::/    /   \\:::\\ ___\\ \n"
           "\t    /:::/  \\:::\\   \\:::|    ||:::|____|     |:::|    |/:: /   "
           " |::|   /::\\____\\/:::/____/  ___\\:::|    |\n"
           "\t    \\::/    \\:::\\  /:::|____| \\:::\\    \\   /:::/    / "
           "\\::/    /|::|  /:::/    /\\:::\\    \\ /\\  /:::|____|\n"
           "\t     \\/_____/\\:::\\/:::/    /   \\:::\\    \\ /:::/    /   "
           "\\/____/ |::| /:::/    /  \\:::\\    /::\\ \\::/    / \n"
           "\t              \\::::::/    /     \\:::\\    /:::/    /           "
           " |::|/:::/    /    \\:::\\   \\:::\\ \\/____/  \n"
           "\t               \\::::/    /       \\:::\\__/:::/    /            "
           " |::::::/    /      \\:::\\   \\:::\\____\\    \n"
           "\t                \\::/____/         \\::::::::/    /              "
           "|:::::/    /        \\:::\\  /:::/    /    \n"
           "\t                 ~~                \\::::::/    /               "
           "|::::/    /          \\:::\\/:::/    /     \n"
           "\t                                    \\::::/    /                "
           "/:::/    /            \\::::::/    /      \n"
           "\t                                     \\::/____/                "
           "/:::/    /              \\::::/    /       \n"
           "\t                                      ~~                      "
           "\\::/    /                \\::/____/        \n"
           "\t                                                               "
           "\\/____/                                  \n"
           "\t                                                                 "
           "                                       \n"

           "\t      $$\\      $$\\           $$\\   $$\\     $$\\   $$\\     "
           "$$\\                                           $$\\ \n"
           "\t      $$$\\    $$$ |          $$ |  $$ |    \\__|  $$ |    $$ "
           "|                                          $$ |\n"
           "\t      $$$$\\  $$$$ |$$\\   $$\\ $$ |$$$$$$\\   $$\\ $$$$$$\\   "
           "$$$$$$$\\   $$$$$$\\   $$$$$$\\   $$$$$$\\   $$$$$$$ |\n"
           "\t      $$\\$$\\$$ $$ |$$ |  $$ |$$ |\\_$$  _|  $$ |\\_$$  _|  "
           "$$  __$$\\ $$  __$$\\ $$  __$$\\  \\____$$\\ $$  __$$ |\n"
           "\t      $$ \\$$$  $$ |$$ |  $$ |$$ |  $$ |    $$ |  $$ |    $$ | "
           " $$ |$$ |  \\__|$$$$$$$$ | $$$$$$$ |$$ /  $$ |\n"
           "\t      $$ |\\$  /$$ |$$ |  $$ |$$ |  $$ |$$\\ $$ |  $$ |$$\\ $$ "
           "|  $$ |$$ |      $$   ____|$$  __$$ |$$ |  $$ |\n"
           "\t      $$ | \\_/ $$ |\\$$$$$$  |$$ |  \\$$$$  |$$ |  \\$$$$  "
           "|$$ |  $$ |$$ |      \\$$$$$$$\\ \\$$$$$$$ |\\$$$$$$$ |\n"
           "\t      \\__|     \\__| \\______/ \\__|   \\____/ \\__|   "
           "\\____/ \\__|  \\__|\\__|       \\_______| \\_______| \\_______|\n\n"

           "\t \t\t\tPlayer 1 your controls are 's' and 'w'                \n"
           "\t \t\t\tPlayer 2 your controls are the arrows of the keyboard \n"
           "\t \t\t\tPush ANY key to start, SPACE to pause, ESC to quit");
  int ch = getch();

  if (ch != 0x1b) {
    erase();

    addWorkTPool(tpool, updateBall, NULL);

    addWorkTPool(tpool, updateBar, &leftBar);

    addWorkTPool(tpool, updateBar, &rightBar);

    addWorkTPool(tpool, updateBackground, NULL);

    waitTPool(tpool);

    pthread_t ballMovementThread;
    pthread_create(&ballMovementThread, NULL, ballMovement, NULL);

    pthread_t refreshScreenThread;
    pthread_create(&refreshScreenThread, NULL, refreshScreen, NULL);

    pthread_t barMovementAndStopThread;
    pthread_create(&barMovementAndStopThread, NULL, barMovementAndStop, NULL);

    pthread_join(ballMovementThread, NULL);
    pthread_join(barMovementAndStopThread, NULL);
    pthread_join(refreshScreenThread, NULL);
  }

  endwin();

  return EXIT_SUCCESS;
}