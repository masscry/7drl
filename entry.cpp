#include "screen.h"
#include "map.h"

object_t player;

void init() {
  scrInit();
  mapInit();

  player.pos.x = world.x / 2;
  player.pos.y = world.y / 2;
}

void cleanup(){
  mapCleanup();
  scrCleanup();
}

int input() {
    switch (getch()){
    case KEY_DOWN:
      if (player.pos.y < map.sz.y - 1){
        player.pos.y += 1;
      }
      break;
    case KEY_UP:
      if (player.pos.y > 0){
        player.pos.y -= 1;
      }
      break;
    case KEY_LEFT:
      if (player.pos.x > 0){
        player.pos.x -= 1;
      }
      break;
    case KEY_RIGHT:
      if (player.pos.x < map.sz.x - 1){
        player.pos.x += 1;
      }
      break;
    case 'w':
      if (map.off.y > 0){
        map.off.y -= 1;
      }
      break;
    case 's':
      if (map.sz.y - map.off.y > world.y + 1){
        map.off.y += 1;
      }
      break;
    case 'a':
      if (map.off.x > 0){
        map.off.x -= 1;
      }
      break;
    case 'd':
      if (map.sz.x - map.off.x > world.x + 1){
        map.off.x += 1;
      }
      break;
    case KEY_END:
      return 0;
    }
    return 1;
}

void draw(){
    erase();
    mapDraw();
    if (  (player.pos.x - map.off.x >= 0)
       && (player.pos.y - map.off.y >= 0)
       && (player.pos.x - map.off.x <= world.x)
       && (player.pos.y - map.off.y <= world.y)){
      mvaddch(player.pos.y - map.off.y, player.pos.x - map.off.x, '@');
    }

    mvprintw(0, 0, "[%d, %d]", player.pos.x, player.pos.y);
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
