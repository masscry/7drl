#include "map.h"
#include "screen.h"

#include <cctype>
#include <cmath>
#include <cassert>

template<typename T>
T min (T a, T b){
  return (a<b)?a:b;
}

template<typename T>
T max (T a, T b){
  return (a<b)?a:b;
}

map_t map;

object_t& mapObject(int x, int y){
  assert(x < map.sz.x);
  assert(y < map.sz.y);
  return map.cl[y * map.sz.x + x];
}

char grass[] = {'.', '\'', ' ', '`', ' ', ' ', '.'};

void mapInit(){
  map.sz.x = world.x*2;
  map.sz.y = world.y*2 + 1;

  map.off.x = 0;
  map.off.y = 0;

  map.cl = (object_t*)calloc(map.sz.x*map.sz.y, sizeof(object_t));

  for (int y = 0; y < map.sz.y; ++y){
    for (int x = 0; x < map.sz.x; ++x){
      object_t& cl = mapObject(x, y);
      cl.pos.x = x;
      cl.pos.y = y;
      cl.smb = grass[rand()%6];
    }
  }
}

void mapCleanup(){
  free(map.cl);
}

void mapDraw(){
  int fx = min(map.sz.x - map.off.x, world.x + 1);
  int fy = min(map.sz.y - map.off.y, world.y + 1);

  for (int y = 0; y < fy; ++y){
    for (int x = 0; x < fx; ++x){
      object_t& obj = mapObject(x + map.off.x, y + map.off.y);
      if (isprint(obj.smb) != 0){
        mvaddch(y, x, obj.smb);
      }
    }
  }

}
