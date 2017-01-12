#include "screen.h"

#include <cmath>
#include <ctime>

coord_t world;

void scrInit() {
  setlocale(LC_ALL, "");

  srand(time(0));

  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  curs_set(0);

  start_color();
  init_pair(CGRASS, COLOR_WHITE, COLOR_GREEN);
  init_pair(CWALL, COLOR_BLACK, COLOR_GREEN);
  init_pair(CZOMBIE, COLOR_BLACK, COLOR_GREEN);
  init_pair(CPLAYER, COLOR_WHITE, COLOR_GREEN);

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

void bline(const coord_t& s, const coord_t& f, void (*callback)(coord_t, void*), void* ptr){
  coord_t xy;
  xy.x = s.x;
  xy.y = s.y;

  int px = f.x - s.x;
  int py = f.y - s.y;

  int e = 2*py - px;
  int i = px;

  callback(xy, ptr);
  while( --i >= 0 ){
    if (e >= 0){
      xy.x += 1;
      xy.y += 1;
      e += 2*(py-px);
    }
    else {
      xy.x += 1;
    }
    e += 2*py;
    callback(xy, ptr);
  }
}
