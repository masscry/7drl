#include "screen.h"

coord_t world;

void scrInit() {
  setlocale(LC_ALL, "");

  initscr();
  halfdelay(1);
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  curs_set(0);

  int maxx = 0;
  int maxy = 0;

  getmaxyx(stdscr, maxy, maxx);

  world.x = maxx - 1;
  world.y = maxy - 1;

}

void scrCleanup() {
  endwin();
}
