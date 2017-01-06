/**
 * @file object.h
 *
 * Common structures.
 */
#pragma once

struct coord_t {
  int x;
  int y;
};

struct object_t {
  coord_t pos;
  char smb;
};

float dist(const coord_t& a, const coord_t& b);
