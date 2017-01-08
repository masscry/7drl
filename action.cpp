#include "action.h"

#include <queue>
#include <vector>

struct actionUnit{
  void (*update)(object_t*);
  object_t* unit;
  float prio;
  float dprio;
};

struct actionUnitCompare{
  bool operator() (const actionUnit* a, const actionUnit* b){
    return a->prio < b->prio;
  }
};

typedef std::priority_queue<actionUnit*, std::vector<actionUnit*>, actionUnitCompare> pqActionUnit_t;

pqActionUnit_t pqAU;

void actionInit(){
  ;
}

void actionCleanup(){
  while(!pqAU.empty()){
    actionUnit* item = pqAU.top();
    delete item;
    pqAU.pop();
  }
}

void actionRegister(object_t* self, float dprio, void (*update)(object_t*)){
  actionUnit* a = new actionUnit;
  a->update = update;
  a->unit = self;
  a->prio = dprio;
  a->dprio = dprio;
  pqAU.push(a);
}

void actionLoop(){
  pqActionUnit_t tPQ;
  while(!pqAU.empty()){ // While queue is not empty
    actionUnit* u = pqAU.top(); // Get top
    pqAU.pop(); // Remove it from queue
    while (u->prio > 1.0f){ // Units with prio > 1.0 can make move
      u->update(u->unit); // Make move
      u->prio -= 1.0f; // Remove one move point
      if (u->prio + u->dprio < pqAU.top()->prio){ // If unit prio on next round < than prio of next unit, break for next.
        break;
      }
    }
    u->prio += u->dprio;
    tPQ.push(u);
  }
  pqAU.swap(tPQ);
}
