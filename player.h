/**
 * @file player.h
 * @author timur
 *
 * Input routines.
 *
 */

#pragma once

int key2side(int key);

bool playerControl(int key);

extern object_t* player;

enum gameMode_t {
  GM_WALK = 0,
  GM_AIM = 1
};

extern int gameMode;
