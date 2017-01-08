/**
 * @file screen.h
 * @author timur
 *
 * Screen routines.
 *
 */

#pragma once

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <clocale>
#include <ncurses.h>

#include "object.h"

/**
 * Initialize console.
 */
void scrInit();

/**
 * Cleanup console.
 */
void scrCleanup();

extern coord_t world;

void bline(const coord_t& s, const coord_t& f, void (*callback)(coord_t, void*), void* ptr);

enum {
  CGRASS = 1,
  CWALL = 2,
  CZOMBIE = 3,
  CPLAYER = 4
};
