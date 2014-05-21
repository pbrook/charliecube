/*
  Swipe a plane accross the cube
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3
 */
#include "ccdemo.h"

class Swipe : public CCDemo
{
private:
  uint8_t phase;
  int8_t pos;
  int8_t delta;
  void set_plane(uint8_t val);
  void set_xplane(uint8_t x, uint8_t val);
  void set_yplane(uint8_t y, uint8_t val);
  void set_zplane(uint8_t z, uint8_t val);
public:
  virtual int reset(void);
  virtual bool tick(void);
};

void Swipe::set_xplane(uint8_t x, uint8_t val)
{
  uint8_t y;
  uint8_t z;
  for (y = 0; y < CUBE_SIZE; y++)
    for (z = 0; z < CUBE_SIZE; z++)
      cube.set_pixel(x, y, z, val);
}

void Swipe::set_yplane(uint8_t y, uint8_t val)
{
  uint8_t x;
  uint8_t z;
  for (x = 0; x < CUBE_SIZE; x++)
    for (z = 0; z < CUBE_SIZE; z++)
      cube.set_pixel(x, y, z, val);
}

void Swipe::set_zplane(uint8_t z, uint8_t val)
{
  uint8_t x;
  uint8_t y;
  for (x = 0; x < CUBE_SIZE; x++)
    for (y = 0; y < CUBE_SIZE; y++)
      cube.set_pixel(x, y, z, val);
}

int
Swipe::reset(void)
{
  phase = 0;
  pos = -1;
  delta = 1;
  cube.clear();
  return 200;
}

void
Swipe::set_plane(uint8_t val)
{
  if (pos < 0)
    return;
  if (phase == 0) {
      set_xplane(pos, val);
  } else if (phase == 1) {
      set_yplane(pos, val);
  } else if (phase == 2) {
      set_zplane(pos, val);
  }
}

bool
Swipe::tick(void)
{
  Serial.println(phase);
  set_plane(0);
  pos += delta;
  if (pos == CUBE_SIZE - 1)
    delta = -1;
  if (pos == -1) {
      phase++;
      if (phase == 3)
	return true;
      pos = 0;
      delta = 1;
  }
  set_plane(0xff);
  return false;
}

DEF_DEMO(Swipe)
