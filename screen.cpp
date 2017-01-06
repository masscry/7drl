#include "screen.h"

#include <cmath>

coord_t world;

void scrInit() {
  setlocale(LC_ALL, "");

  initscr();
  cbreak();
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

float dist(const coord_t& a, const coord_t& b){
  return sqrtf((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}
