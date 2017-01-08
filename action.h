/**
 * @file action.h
 * @author timur
 *
 * Action queue routines.
 *
 */

#pragma once

#include "object.h"
#include "screen.h"
#include "map.h"

void actionInit();
void actionCleanup();

void actionRegister(object_t* self, float dprio, void (*update)(object_t*));

void actionLoop();
