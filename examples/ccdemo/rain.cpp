/*
  CharlieCube rain
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3
 */
 
#include "ccdemo.h"

#define MAX_DROPS 16

#define MAX_Z (CUBE_SIZE + 1)

#if MAX_DROPS > CUBE_SIZE * CUBE_SIZE
#error barf
#endif

class RainDrop
{
public:
  uint8_t x;
  uint8_t y;
  uint8_t z;
  uint8_t speed;
  uint8_t tick_count;
  RainDrop(): x(0), y(0), z(MAX_Z) {}
  RainDrop(uint8_t new_x, uint8_t new_y, uint8_t new_speed): x(new_x), y(new_y), z(0), speed(new_speed) {}
  void tick(void);
  bool valid(void) { return z < MAX_Z; };
};

class Rain : public CCDemo
{
private:
  RainDrop drop[MAX_DROPS];
  void respawn(int n);

public:
  virtual int reset(void);
  virtual bool tick(void);
};

void
RainDrop::tick(void)
{
  int i;
  int bright;

  if (!valid())
    return;

  tick_count++;
  if (tick_count == speed) {
      tick_count = 0;
      z++;
  }
  bright = 0xff;
  for (i = CUBE_SIZE - z; i < CUBE_SIZE; i++) {
      if (i >= 0)
	cube.set_pixel(x, y, i, bright);
      bright -= 0xff;
      if (bright < 0)
	bright = 0;
  }
}

int
Rain::reset(void)
{
  int i;

  cube.clear();
  for (i = 0; i < MAX_DROPS; i++) {
      drop[i] = RainDrop();
  }
  return 100;
}

void
Rain::respawn(int n)
{
  uint8_t x;
  uint8_t y;
  int i;

  while (true) {
      x = random(CUBE_SIZE);
      y = random(CUBE_SIZE);

      for (i = 0; i < MAX_DROPS; i++) {
	  if (drop[i].valid() && drop[i].x == x && drop[i].y == y)
	    continue;
      }
      break;
  }
  drop[n] = RainDrop(x, y, random(3) + 2);
}

bool
Rain::tick(void)
{
  int i;
  int spawn = 1;

  for (i = 0; i < MAX_DROPS; i++) {
      if (spawn && !drop[i].valid()) {
	  respawn(i);
	  spawn--;
      }
      drop[i].tick();
  }
  return false;
}

DEF_DEMO(Rain)
