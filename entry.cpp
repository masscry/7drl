#include "screen.h"
#include "map.h"
#include "clock.h"
#include "player.h"
#include "config.h"

object_t* actor = 0;
int actorCount = 1;

void init() {
  scrInit();
  mapInit();
  configInit();
  clockStart();

  const char* cntKey = configKey("TOTAL_ACTORS");
  if (cntKey != 0){
    actorCount = atoi(cntKey);
  }
  actor = (object_t*)calloc(actorCount, sizeof(object_t));

  player = actor;

  player->pos.x = world.x / 2;
  player->pos.y = world.y / 2;
  player->smb = '@';

  for (int i = 1; i < actorCount; ++i)
  {
    actor[i].pos.x = rand()%map.sz.x;
    actor[i].pos.y = rand()%map.sz.y;
    actor[i].smb = 'Z';
  }

}

void cleanup(){
  free(actor);
  mapCleanup();
  scrCleanup();
}

int input() {
    int smb = getch();
    if (playerControl(smb)){
      return 1;
    }
    if (mapControl(smb)){
      return 1;
    }
    switch (smb){
    case KEY_END:
      return 0;
    }
    return 1;
}

void draw(){
    erase();
    mapHeatAdd(5.0f, player->pos);
    mapDraw();
    for (int i = 0; i < actorCount; ++i)
    {
      if (  (actor[i].pos.x - map.off.x >= 0)
         && (actor[i].pos.y - map.off.y >= 0)
         && (actor[i].pos.x - map.off.x <= world.x)
         && (actor[i].pos.y - map.off.y <= world.y)){
        mvaddch(actor[i].pos.y - map.off.y, actor[i].pos.x - map.off.x, actor[i].smb);
      }
    }
    refresh();
}

void update(){
  for (int i = 1; i < actorCount; ++i)
  {
    float dist = 1.0e9f;
    int bsd = 10;
    for (int sd = 0; sd < SIDE_COUNT; ++sd){
      if (mapCanPass(actor[i].pos, sd)){
        float tdst = mapHeat(actor[i].pos.x + sides[sd].x , actor[i].pos.y + sides[sd].y);
        if (tdst < dist){
          dist = tdst;
          bsd = sd;
        }
      }
    }
    actor[i].pos.x += sides[bsd].x;
    actor[i].pos.y += sides[bsd].y;
  }
}

int main(int argc, char* argv[]){
  init();
  do{
    if (gameMode == GM_WALK){
      update();
    }
    draw();
  }while(input());
  cleanup();
  return 0;
}
