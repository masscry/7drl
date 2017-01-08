#include <cassert>
#include "screen.h"
#include "player.h"
#include "map.h"

int key2side(int key){
  switch(key){
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

int gameMode = GM_WAIT;


bool walkControl(int key, object_t* player){

  switch(key){
    case KEY_UP:
    case KEY_DOWN:
    case KEY_LEFT:
    case KEY_RIGHT:
    case KEY_A1:
    case KEY_A3:
    case KEY_C1:
    case KEY_C3:
    {
      int side = key2side(key);
      if (mapCanPass(player->pos, side)){
        player->pos.x += sides[side].x;
        player->pos.y += sides[side].y;
      }
      return true;
    }
  }
  return false;
}

bool playerControl(int key, object_t* player){
  return walkControl(key, player);
}

void zombieUpdate(object_t* zombie){

  if (gameMode != GM_STEP){
    return;
  }

  float dist = 1.0e9f;
  int bsd = 10;
  for (int sd = 0; sd < SIDE_COUNT; ++sd){
    if (mapCanPass(zombie->pos, sd)){
      float tdst = mapHeat(zombie->pos.x + sides[sd].x, zombie->pos.y + sides[sd].y);
      if (tdst < dist){
        dist = tdst;
        bsd = sd;
      }
    }
  }
  zombie->pos.x += sides[bsd].x;
  zombie->pos.y += sides[bsd].y;
}

extern int run;

int input(object_t* player){
  int smb = getch();
  if (playerControl(smb, player)){
    gameMode = GM_STEP;
    return 1;
  }
  if (mapControl(smb)){
    return 1;
  }
  switch(smb){
  case KEY_END:
    return 0;
  }
  return 1;
}

void playerUpdate(object_t* plr){
  run = input(plr);
}
