#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <clocale>
#include <ncurses.h>

struct object_t {
  int x;
  int y;
};

object_t world;
object_t player;


int init() {
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

  player.x = world.x / 2;
  player.y = world.y / 2;
}

void cleanup(){
  endwin();
}

int input() {
    switch (getch()){
    case KEY_DOWN:
      if (player.y < world.y){
        player.y += 1;
      }
      break;
    case KEY_UP:
      if (player.y > 0){
        player.y -= 1;
      }
      break;
    case KEY_LEFT:
      if (player.x > 0){
        player.x -= 1;
      }
      break;
    case KEY_RIGHT:
      if (player.x < world.x){
        player.x += 1;
      }
      break;
    case KEY_END:
      return 0;
    }
    return 1;
}

void draw(){
    erase();
    mvaddch(player.y, player.x, '@');
    refresh();
}

int main(int argc, char* argv[]){
  init();
  while (input()){
    draw();
  }
  cleanup();
  return 0;
}
