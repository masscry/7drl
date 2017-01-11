#include "screen.h"
#include "map.h"
#include "clock.h"
#include "player.h"
#include "config.h"
#include "action.h"
#include "error.h"

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

  player->top = 0;
  player->pos.x = world.x / 2;
  player->pos.y = world.y / 2;
  player->smb = '@';
  player->col = COLOR_PAIR(CPLAYER) | A_BOLD;
  actionRegister(player, 1.0f, playerUpdate);

  for (int i = 1; i < actorCount; ++i)
  {
    actor[i].top = 0;
    actor[i].pos.x = rand()%map.sz.x;
    actor[i].pos.y = rand()%map.sz.y;
    actor[i].smb = 'Z';
    actor[i].col = COLOR_PAIR(CZOMBIE) | A_BOLD;
    actionRegister(actor+i, 0.5f, zombieUpdate);
    object_t& mcell = mapObject(actor[i].pos.x, actor[i].pos.y);
    mcell.top = actor+i;
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
    mapHeatAdd(actor->pos, 5.0f);
    mapHeatObjects(actor, actorCount);
    mapDraw(actor, 10.0f);
    mapDrawActors(actor, actorCount);
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
