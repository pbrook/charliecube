/*
  Fade the whole cube on and off
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3
 */
#include "ccdemo.h"

class Fade : public CCDemo
{
private:
  int bright;
  int delta;
public:
  virtual int reset(void);
  virtual bool tick(void);
};

void set_xplane(uint8_t x, uint8_t val)
{
  uint8_t y;
  uint8_t z;
  for (y = 0; y < 4; y++)
    for (z = 0; z < 4; z++)
      cube.set_pixel(x, y, z, val);
}

int
Fade::reset(void)
{
  bright = 0xf;
  delta = 0x10;
  return 50;
}

bool
Fade::tick(void)
{
  int x, y, z;

  for (x = 0; x < 4; x++)
    for (y = 0; y < 4; y++)
      for (z = 0; z < 4; z++)
	cube.set_pixel(x, y, z, bright);
  bright += delta;
  if (bright >= 0x100) {
      bright = 0xff;
      delta = -delta;
  } else if (bright < 0) {
      return true;
  }
  return false;
}

DEF_DEMO(Fade)
