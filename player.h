/**
 * @file player.h
 * @author timur
 *
 * Input routines.
 *
 */

#pragma once

int key2side(int key);

enum gameMode_t {
  GM_WAIT = 0,
  GM_STEP = 1,
};

extern int gameMode;

void playerUpdate(object_t*);
void zombieUpdate(object_t*);
