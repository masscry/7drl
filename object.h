/**
 * @file object.h
 *
 * Common structures.
 */
#pragma once

#include <ncurses.h>

struct coord_t {
  int x;
  int y;
};

struct object_t {
  coord_t pos;
  chtype smb;
  chtype col;
  object_t* top;
};

float dist(const coord_t& a, const coord_t& b);
