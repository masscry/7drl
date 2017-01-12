/**
 * @file map.h
 * @author timur
 *
 * Global map routines.
 *
 */

#pragma once

#include <cstdio>
#include "object.h"

struct map_t {
  coord_t off;
  coord_t sz;
  object_t* cl;
  float* heat;
};

extern map_t map;

void mapInit();
void mapCleanup();
void mapDraw(const object_t* center, float dist);
void mapDrawActors(object_t* act, size_t actsize, float dist);

object_t& mapObject(int x, int y);
float& mapHeat(int x, int y);

void mapHeatAdd(coord_t crd, float maxdist);
void mapHeatObjects(object_t* act, size_t actsize);



void mapHeatDebug(FILE* output);

#define SIDE_COUNT (8)

enum {
  SIDE_RIGHT = 0,
  SIDE_RIGHT_BOTTOM,
  SIDE_BOTTOM,
  SIDE_LEFT_BOTTOM,
  SIDE_LEFT,
  SIDE_LEFT_TOP,
  SIDE_TOP,
  SIDE_RIGHT_TOP
};

extern coord_t sides[SIDE_COUNT];
extern float sidecost[SIDE_COUNT];

bool mapInside(const coord_t& crd);
bool mapCanPass(const coord_t& crd, int sd);

bool mapControl(int key);
