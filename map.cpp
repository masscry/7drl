#include "map.h"
#include "screen.h"
#include "clock.h"

#include <cctype>
#include <cmath>
#include <cassert>
#include <queue>
#include <vector>
#include <cstring>

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
  assert(x >= 0);
  assert(y >= 0);
  assert(x < map.sz.x);
  assert(y < map.sz.y);
  return map.cl[y * map.sz.x + x];
}

float& mapHeat(int x, int y){
  assert(x >= 0);
  assert(y >= 0);
  assert(x < map.sz.x);
  assert(y < map.sz.y);
  return map.heat[y * map.sz.x + x];
}

char grass[] = {'.', '\'', ' ', '"', ' ', ' ', '.', '#'};

void mapInit(){
  map.sz.x = world.x*2;
  map.sz.y = world.y*2 + 1;

  map.off.x = 0;
  map.off.y = 0;

  map.cl = (object_t*)calloc(map.sz.x*map.sz.y, sizeof(object_t));
  map.heat = (float*)calloc(map.sz.x*map.sz.y, sizeof(float));

  for (int y = 0; y < map.sz.y; ++y){
    for (int x = 0; x < map.sz.x; ++x){
      object_t& cl = mapObject(x, y);
      float& ht = mapHeat(x, y);
      ht = 0.0f;
      cl.pos.x = x;
      cl.pos.y = y;
      cl.smb = grass[rand()%8];
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
      mvaddch(y, x, obj.smb);
    }
  }

}

coord_t sides[SIDE_COUNT] = {
  { 1, 0},
  { 1, 1},
  { 0, 1},
  {-1, 1},
  {-1, 0},
  {-1,-1},
  { 0,-1},
  { 1,-1}
};

float sidecost[SIDE_COUNT] = {
  1.0f,
  1.4142135623730951f,
  1.0f,
  1.4142135623730951f,
  1.0f,
  1.4142135623730951f,
  1.0f,
  1.4142135623730951f,
};

bool mapInside(const coord_t& crd){
  return (crd.x >= 0) && (crd.y >= 0) && (crd.x < map.sz.x) && (crd.y < map.sz.y);
}

bool mapCanPass(const coord_t& crd, int sd){
  assert(crd.x >= 0);
  assert(crd.y >= 0);
  assert(crd.x < map.sz.x);
  assert(crd.y < map.sz.y);

  if (crd.x + sides[sd].x < 0 ){
    return false;
  }

  if (crd.x + sides[sd].x >= map.sz.x){
    return false;
  }

  if (crd.y + sides[sd].y < 0 ){
    return false;
  }

  if (crd.y + sides[sd].y >= map.sz.y){
    return false;
  }

  object_t& dst = mapObject(crd.x + sides[sd].x, crd.y + sides[sd].y);
  if (dst.smb == '#'){
    return false;
  }

  if (sidecost[sd] > 1.0f){
    object_t& ds1 = mapObject(crd.x + sides[sd].x, crd.y);
    object_t& ds2 = mapObject(crd.x, crd.y + sides[sd].y);
    return ((ds1.smb != '#')&&(ds2.smb != '#'));
  }

  return true;
}

void mapHeatAdd(float val, coord_t crd){
  assert(crd.x >= 0);
  assert(crd.y >= 0);
  assert(crd.x < map.sz.x);
  assert(crd.y < map.sz.y);

  std::vector<float> dist;
  dist.resize(map.sz.x*map.sz.y, 1.0e9f);
  dist[crd.x + crd.y*map.sz.x] = 0.0f;

  std::queue<coord_t> wave;
  wave.push(crd);

  while(!wave.empty()){
    coord_t root;
    root = wave.front();
    wave.pop();
    for (int sd = 0; sd < SIDE_COUNT; ++sd ){
      if (mapCanPass(root, sd))
      {
        coord_t nd;
        nd.x = root.x + sides[sd].x;
        nd.y = root.y + sides[sd].y;
        object_t& tile = mapObject(nd.x, nd.y);
        if ((dist[root.x + root.y*map.sz.x] + sidecost[sd]) <  dist[nd.x + nd.y*map.sz.x]){
          dist[nd.x + nd.y*map.sz.x] = dist[root.x + root.y*map.sz.x] + sidecost[sd];
          wave.push(nd);
        }
      }
    }
  }
  memcpy(map.heat, dist.data(), sizeof(float)*map.sz.x*map.sz.y);
}

void mapHeatDebug(FILE* output){
  fprintf(output, "[%g];\n;", clockNow());
  for (int y = 0; y < map.sz.y; ++y){
    for (int x = 0; x < map.sz.x; ++x){
      float& heat = mapHeat(x, y);
      fprintf(output, "%f;", heat);
    }
    fprintf(output, "\n");
  }
  fprintf(output, "===");
}
