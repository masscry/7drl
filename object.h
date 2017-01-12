/**
 * @file object.h
 *
 * Common structures.
 */
#pragma once

#ifndef _WIN32
#include <ncurses.h>
#else
#include "curses.h"
#endif // _WIN32

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
