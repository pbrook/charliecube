/*
  CharlieCube worm
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3
 */
 
#include "ccdemo.h"

#define BODY_SIZE 16

#define CUBE_SIZE 4

#define IQ 10

#define D 100

#define INITIAL_ENERGY 0xc0
#define ENERGY_DECAY 0x10

#define BODY_MASK (BODY_SIZE - 1)
#if BODY_SIZE & BODY_MASK
#error BODY_SIZE must be a power of two
#endif

#if (INITIAL_ENERGY / ENERGY_DECAY + 2) >= BODY_SIZE
#error BODY_SIZE too small
#endif

class Position
{
public:
  int8_t x;
  int8_t y;
  int8_t z;
  Position(): x(0), y(0), z(0) {}
  Position(int x_, int y_, int z_): x(x_), y(y_), z(z_) { }
  void advance(int dir, bool forward) {
      int delta = forward ? 1 : -1;
      if (dir == 0)
	x += delta;
      else if (dir == 1)
	y += delta;
      else
	z += delta;
  }
  bool operator== (const Position & other) const {
      return x == other.x && y == other.y && z == other.z;
  }
};

class Worm : public CCDemo
{
  uint8_t dir;
  int tries;
  bool forward;
  bool blocked;
  Position current_pos;

  uint8_t head;
  uint8_t tail;
  Position body[BODY_SIZE];
  uint8_t energy[BODY_SIZE];
public:
  bool is_empty(const Position &p);
  void push(const Position &p);
  void age();
public:
  virtual int reset(void);
  virtual bool tick(void);
};

bool
Worm::is_empty(const Position &p)
{
  uint8_t i;

  if (p.x < 0 || p.x >= CUBE_SIZE)
    return false;
  if (p.y < 0 || p.y >= CUBE_SIZE)
    return false;
  if (p.z < 0 || p.z >= CUBE_SIZE)
    return false;

  for (i = tail; i != head; i = (i + 1) & BODY_MASK) {
      if (p == body[i])
	return false;
  }
  return true;
}

void
Worm::push(const Position & p)
{
  body[head] = p;
  energy[head] = INITIAL_ENERGY;
  head = (head + 1) & BODY_MASK;
}

void
Worm::age()
{
  int i;

  for (i = tail; i != head; i = (i + 1) & BODY_MASK) {
      Position& p = body[i];
      if (energy[i] > ENERGY_DECAY) {
	  energy[i] -= ENERGY_DECAY;
      } else {
	  energy[i] = 0;
	  tail = (tail + 1) & BODY_MASK;
      }
      cube.set_pixel(p.x, p.y, p.z, energy[i]);
  }
}

int
Worm::reset(void)
{
  current_pos = Position(0,0,0);
  dir = 0;
  tries = 0;
  forward = true;
  blocked = true;
  return 100;
}

bool
Worm::tick(void)
{
  while (true) {
      Position new_pos = current_pos;
      if (blocked || random(4) != 0) {
	  dir = random(3);
	  forward = (random(2) == 0);
      }
      new_pos.advance(dir, forward);
      if (is_empty(new_pos)) {
	  current_pos = new_pos;
	  cube.set_pixel(current_pos.x, current_pos.y, current_pos.z, 0xff);
	  push(current_pos);
	  tries += IQ;
	  blocked = false;
      } else {
	  blocked = true;
	  tries++;
      }
      if (tries >= IQ) {
	  age();
	  tries = 0;
	  return false;
      }
  }
}

DEF_DEMO(Worm)
