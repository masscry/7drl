#include "screen.h"
#include "map.h"
#include "clock.h"
#include "player.h"
#include "config.h"
#include "action.h"
#include "error.h"

object_t* actor = 0;
int actorCount = 1;
float actSpd = 1.0f;
float zSpd = 0.7f;

int run = 1;
float stepLength = 0.0f;

void init() {
  scrInit();
  mapInit();
  configInit();
  actionInit();
  clockStart();

  const char* cntKey = configKey("TOTAL_ACTORS");
  const char* actSpdKey = configKey("MIKLE_SPEED");
  const char* zSpdKey = configKey("Z_SPEED");

  if (cntKey != 0){
    actorCount = atoi(cntKey);
  }

  if (actSpdKey != 0){
    actSpd = atoi(actSpdKey)/100.0f;
  }

  if (zSpdKey != 0){
    zSpd = atoi(zSpdKey)/100.0f;
  }

  actor = (object_t*)calloc(actorCount, sizeof(object_t));

  object_t* player = actor;

  player->top = 0;
  player->pos.x = world.x / 2;
  player->pos.y = world.y / 2;
  player->smb = '@';
  player->col = COLOR_PAIR(CPLAYER) | A_BOLD;
  actionRegister(player, actSpd, playerUpdate);

  for (int i = 1; i < actorCount; ++i)
  {
    actor[i].top = 0;
    actor[i].pos.x = rand()%map.sz.x;
    actor[i].pos.y = rand()%map.sz.y;
    actor[i].smb = 'Z';
    actor[i].col = COLOR_PAIR(CZOMBIE) | A_BOLD;
    actionRegister(actor+i, zSpd, zombieUpdate);
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
  mapDraw(actor, 10.0f);
  mapDrawActors(actor, actorCount, 10.0f);
  refresh();
}

const char* text[] = {
  "September 10, 1979",
  "",
  "Dear Loris,",
  "",
  "If you received this letter, then something horrible had happend to me.",
  "I beg you! You must promise me to never come after me.",
  "There is no way you can save me.",
  "I've seen The Unspeakable... and finally it hunted me down.",
  "",
  "Yours forever, Mikle.",
  0
};


void intro(){
  erase();
  const char** cursor = text;

  int y = 3;
  while(*cursor!=0){
    mvprintw(y, 0, "%s", *cursor);
    ++y;
    ++cursor;
  }
  refresh();
}

const char* etext[] = {
  "Masscry's 7DRL \"Do not stop\"",
  "",
  "Mikle Walkers was found dead without traces of violence or gunshot wounds.", // 0
  "No one knows why he acted so strangely last night.", // 10
  "People says that he didn't sleep well lately.", // 20
  "His feancee Loris Stern gone missing two years later in the same mountains.", // 50
  "Her parents said that she received a letter from Mikle two days after", // 100
  "his body was found.", // 100
  "She refused to believe that he was dead and talked to everyone who have seen",
  "him during the final year.", // 200
  "People says that she found some crazy old man, who told her some story and",
  "gave her Mikle's dairy.", // 500
  "The day she dissapeared in the mountains",
  "there was a heavy rain and thunderstorm all night long.", // 1000
  "A lone traveller witnessed, that he saw",
  "a woman on the top of the mountain.",
  "this woman carried some kind of ritual.", // 2000
  "And then she dissapeared in the bolt of lightning.", // 5000
  "After that night country folks' children started to dissappear.",
  "The locals says that in stormy nights one can see two dancing",
  "figures on the hilltop. They are Mikle and Loris together forever...", // 10000
  0
};

float lvls[] = {
    0.0f,
    0.0f,
    0.0f,
    10.0f,
    20.0f,
    50.0f,
   100.0f,
   100.0f,
   200.0f,
   200.0f,
   500.0f,
   500.0f,
  1000.0f,
  1000.0f,
  2000.0f,
  2000.0f,
  2000.0f,
  5000.0f,
 10000.0f,
 10000.0f,
 10000.0f,
   1.0e9f
};

void extra(){
  erase();
  const char** cursor = etext;

  int line = 0;
  while(*cursor != 0){
    if (lvls[line] > stepLength){
      break;
    }
    mvprintw(line, 0, "%s", *cursor);
    ++line;
    ++cursor;
  }

  mvprintw(line + 2, 0, "Total distance: %3.1f", stepLength);

  refresh();
}

int main(int argc, char* argv[]){
  init();

  intro();
  getch();

  do{
    actionLoop();
    gameMode = GM_WAIT;
    draw();

    for (int i = 1; i<actorCount; ++i){
      if (dist(actor[0].pos,actor[i].pos) < 2.0f){
        run = 0;
        extra();
        getch();
      }
    }

  }while(run);
  cleanup();
  return 0;
}
