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

object_t* player = 0;
int gameMode = GM_WALK;


bool walkControl(int key){
  assert(player);

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

bool aimControl(int key){
  return false;
}

bool playerControl(int key){

  switch (key){
    case '`':
    {
      switch (gameMode){
      case GM_WALK:
        gameMode = GM_AIM;
        return true;
      case GM_AIM:
        gameMode = GM_WALK;
        return true;
      }
    }
  }

  switch (gameMode){
    case GM_WALK:
      return walkControl(key);
    case GM_AIM:
      return aimControl(key);
  }

  return false;
}
