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
