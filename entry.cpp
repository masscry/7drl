#include "screen.h"
#include "map.h"
#include "clock.h"

object_t player;

#define ZOMBIE_COUNT (50)

object_t zombie[ZOMBIE_COUNT];

void init() {
  scrInit();
  mapInit();
  clockStart();

  player.pos.x = world.x / 2;
  player.pos.y = world.y / 2;
  player.smb = '@';

  for (int i = 0; i < ZOMBIE_COUNT; ++i)
  {
    zombie[i].pos.x = rand()%map.sz.x;
    zombie[i].pos.y = rand()%map.sz.y;
    zombie[i].smb = 'Z';
  }

}

void cleanup(){
  mapCleanup();
  scrCleanup();
}

int key2side(int key){
  switch (key){
  case KEY_UP:
    return SIDE_TOP;
  case KEY_DOWN:
    return SIDE_BOTTOM;
  case KEY_LEFT:
    return SIDE_LEFT;
  case KEY_RIGHT:
    return SIDE_RIGHT;
  case KEY_A1:
    return SIDE_LEFT_TOP;
  case KEY_A3:
    return SIDE_RIGHT_TOP;
  case KEY_C1:
    return SIDE_LEFT_BOTTOM;
  case KEY_C3:
    return SIDE_RIGHT_BOTTOM;
  }
}

int input() {
    int smb = getch();
    switch (smb){
    case KEY_UP:
    case KEY_DOWN:
    case KEY_LEFT:
    case KEY_RIGHT:
    case KEY_A1:
    case KEY_A3:
    case KEY_C1:
    case KEY_C3:
      {
        int side = key2side(smb);
        if (mapCanPass(player.pos, side)){
          player.pos.x += sides[side].x;
          player.pos.y += sides[side].y;
        }
        break;
      }
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
    mapHeatAdd(5.0f, player.pos);
    mapDraw();
    if (  (player.pos.x - map.off.x >= 0)
       && (player.pos.y - map.off.y >= 0)
       && (player.pos.x - map.off.x <= world.x)
       && (player.pos.y - map.off.y <= world.y)){
      mvaddch(player.pos.y - map.off.y, player.pos.x - map.off.x, '@');
    }

    for (int i = 0; i < ZOMBIE_COUNT; ++i)
    {
      if (  (zombie[i].pos.x - map.off.x >= 0)
         && (zombie[i].pos.y - map.off.y >= 0)
         && (zombie[i].pos.x - map.off.x <= world.x)
         && (zombie[i].pos.y - map.off.y <= world.y)){
        mvaddch(zombie[i].pos.y - map.off.y, zombie[i].pos.x - map.off.x, zombie[i].smb);
      }
    }
    refresh();
}

void update(){
  for (int i = 0; i < ZOMBIE_COUNT; ++i)
  {
    float dist = 1.0e9f;
    int bsd = 10;
    for (int sd = 0; sd < SIDE_COUNT; ++sd){
      if (mapCanPass(zombie[i].pos, sd)){
        float tdst = mapHeat(zombie[i].pos.x + sides[sd].x , zombie[i].pos.y + sides[sd].y);
        if (tdst < dist){
          dist = tdst;
          bsd = sd;
        }
      }
    }
    zombie[i].pos.x += sides[bsd].x;
    zombie[i].pos.y += sides[bsd].y;
  }
}

int main(int argc, char* argv[]){
  init();
  do{
    update();
    draw();
  }while(input());
  cleanup();
  return 0;
}
