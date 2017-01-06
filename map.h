/**
 * @file map.h
 * @author timur
 *
 * Global map routines.
 *
 */

#pragma once

#include "object.h"

struct map_t {
  coord_t off;
  coord_t sz;
  object_t* cl;
};

extern map_t map;

void mapInit();
void mapCleanup();
void mapDraw();

object_t& mapObject(int x, int y);
