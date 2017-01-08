#include "screen.h"
#include "map.h"
#include "clock.h"
#include "player.h"
#include "config.h"
#include "action.h"

object_t* actor = 0;
int actorCount = 1;
int run = 1;

void init() {
  scrInit();
  mapInit();
  configInit();
  actionInit();
  clockStart();

  const char* cntKey = configKey("TOTAL_ACTORS");
  if (cntKey != 0){
    actorCount = atoi(cntKey);
  }
  actor = (object_t*)calloc(actorCount, sizeof(object_t));

  object_t* player = actor;

  player->pos.x = world.x / 2;
  player->pos.y = world.y / 2;
  player->smb = '@';
  actionRegister(player, 1.0f, playerUpdate);

  for (int i = 1; i < actorCount; ++i)
  {
    actor[i].pos.x = rand()%map.sz.x;
    actor[i].pos.y = rand()%map.sz.y;
    actor[i].smb = 'Z';
    actionRegister(actor+i, 0.5f, zombieUpdate);
  }

}

void cleanup(){
  actionCleanup();
  free(actor);
  mapCleanup();
  scrCleanup();
}

void draw(){
    erase();
    mapHeatAdd(5.0f, actor->pos);
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

int main(int argc, char* argv[]){
  init();
  do{
    actionLoop();
    gameMode = GM_WAIT;
    draw();
  }while(run);
  cleanup();
  return 0;
}
