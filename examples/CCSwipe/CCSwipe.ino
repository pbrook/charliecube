/*
  CharlieCube swipe example
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3

  Swipe a plane accross the x, y and z axes.
 */
 
#include "charliecube.h"

#define D 200

CharlieCube cube;

void setup()
{
  cube.begin();
}

void set_xplane(uint8_t x, uint8_t val)
{
  uint8_t y;
  uint8_t z;
  for (y = 0; y < 4; y++)
    for (z = 0; z < 4; z++)
      cube.set_pixel(x, y, z, val);
}

void set_yplane(uint8_t y, uint8_t val)
{
  uint8_t x;
  uint8_t z;
  for (x = 0; x < 4; x++)
    for (z = 0; z < 4; z++)
      cube.set_pixel(x, y, z, val);
}

void set_zplane(uint8_t z, uint8_t val)
{
  uint8_t x;
  uint8_t y;
  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
      cube.set_pixel(x, y, z, val);
}

void loop()
{
  int8_t i;
  for (i = 0; i < 4; i++) {
    set_xplane(i, 0xff);
    delay(D);
    set_xplane(i, 0);
  }
  for (i = 2; i >= 0;  i--) {
    set_xplane(i, 0xff);
    delay(D);
    set_xplane(i, 0);
  }
  delay(D);
  for (i = 0; i < 4; i++) {
    set_yplane(i, 0xff);
    delay(D);
    set_yplane(i, 0);
  }
  for (i = 2; i >= 0; i--) {
    set_yplane(i, 0xff);
    delay(D);
    set_yplane(i, 0);
  }
  delay(D);
  for (i = 0; i < 4; i++) {
    set_zplane(i, 0xff);
    delay(D);
    set_zplane(i, 0);
  }
  for (i = 2; i >= 0; i--) {
    set_zplane(i, 0xff);
    delay(D);
    set_zplane(i, 0);
  }
  delay(D);
}

