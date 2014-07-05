/*
  CharlieCube demo sketch
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3
 */
 
#include <TimerOne.h>
#include <charliecube.h>
#include "ccdemo.h"

// Advance to next demo after this many seconds
#define MAX_DEMO_TIME 10

#define DEMO(name) void register_##name(void);
#include "ccdemo.def"
#undef DEMO

CharlieCube2 real_cube;

CharlieCubeBase &cube = real_cube;

typedef enum {
    SM_IDLE,
    SM_READY,
    SM_ACTIVE
} serial_mode;

void setup()
{
  Serial.begin(9600);
  // Not great, but better than nothing
  randomSeed(analogRead(0));
#define DEMO(name) register_##name();
#include "ccdemo.def"
#undef DEMO
  real_cube.begin();
  real_cube.single_buffer();
}

static CCDemo *all_demos[] = {
#define DEMO(name) NULL,
#include "ccdemo.def"
#undef DEMO
};

static uint8_t num_demos;

void
register_demo(CCDemo *demo)
{
  all_demos[num_demos++] = demo;
}

static void
set_frames(uint8_t new_display, uint8_t new_write)
{
  static uint8_t display_frame;
  static uint8_t write_frame;
  if (display_frame == new_display && write_frame == new_write)
    return;
  if (display_frame == new_display) {
      if (new_display == new_write) {
	  real_cube.single_buffer();
      } else {
	  real_cube.swap();
      }
  } else {
      if (display_frame == write_frame)
	real_cube.swap();
      real_cube.swap();
      if (new_display == new_write)
	real_cube.single_buffer();
  }
  real_cube.wait();
  display_frame = new_display;
  write_frame = new_write;
}

static void run_demo(CCDemo *demo)
{
  bool finished;
  unsigned long last_tick;
  uint16_t period;
  long expires;

  set_frames(0, 0);
  period = demo->reset();
  finished = false;
  last_tick = millis();
  expires = MAX_DEMO_TIME * 1000l;
  while (!finished) {
      finished = demo->tick();
      while (millis() - last_tick < period)
	/* no-op */;
      last_tick += period;
      expires -= period;
      if (expires <= 0)
	finished = true;
      if (Serial.available())
	finished = true;
  }
}

static bool
do_data(void)
{
  static uint8_t n;
  static uint8_t cmd;
  static uint8_t d0;
  static uint8_t d1;
  static uint8_t d2;
  static serial_mode sm = SM_IDLE;

  if (!Serial.available())
    return false;
  if (n == 0) {
      cmd = Serial.read();
      if (cmd == 0xff)
	return true;
      n++;
      return true;
  } else if (n == 1) {
      d0 = Serial.read();
      n++;
      return true;
  } else if (n == 2) {
      d1 = Serial.read();
      n++;
      return true;
  }
  d2 = Serial.read();
  n = 0;
  if (cmd == 0xe0) {
      if (d0 == 0xf0 && d1 == 0xf1 && d2 == 0xf2)
	sm = SM_READY;
      else
	sm = SM_IDLE;
      return true;
  }
  if (cmd < 0xd0 && sm != SM_ACTIVE) {
      return true;
  }
  if (sm == SM_IDLE)
    return true;
  if (cmd < 4*4*4) {
      uint8_t x = cmd & 3;
      uint8_t y = (cmd >> 2) & 3;
      uint8_t z = (cmd >> 4) & 3;
      cube.set_pixel(x, y, z, d0 | d1 | d2);
  } else if (cmd == 0x80u) {
      /* Set framebuffer (page flip).  */
      set_frames(d1 & 1, d2 & 1);
  } else if (cmd == 0xc0) {
      /* Set brightness (ignored) */
  } else if (cmd == 0xe1) {
      if (d0 == 0 || d0 == 0xff)
	sm = SM_ACTIVE;
      else
	sm = SM_READY;
  } else {
      /* Unknown command.  */
      sm = SM_IDLE;
  }
  return true;
}

void loop()
{
  int n;
  long last_data;
  long now;

  last_data = millis() - 900;
  while (true) {
      now = millis();
      if (do_data())
	  last_data = now;
      if (now - last_data < 1000)
	continue;
      real_cube.single_buffer();
      for (n = 0; n < num_demos; n++) {
	  run_demo(all_demos[n]);
      }
  }
}
