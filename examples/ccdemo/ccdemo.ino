/*
  CharlieCube demo sketch
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3
 */
 
#include <TimerOne.h>
#include <charliecube.h>
#include "ccdemo.h"


#define DEMO(name) void register_##name(void);
#include "ccdemo.def"
#undef DEMO

CharlieCube real_cube;

CharlieCubeBase &cube = real_cube;

void setup()
{
  Serial.begin(9600);
#define DEMO(name) register_##name();
#include "ccdemo.def"
#undef DEMO
  real_cube.begin();
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

static void run_demo(CCDemo *demo)
{
  bool finished;
  unsigned long last_tick;
  uint16_t period;

  period = demo->reset();
  finished = false;
  last_tick = millis();
  while (!finished) {
      finished = demo->tick();
      while (millis() - last_tick < period)
	/* no-op */;
      last_tick += period;
  }
}

void loop()
{
  int n;

  for (n = 0; n < num_demos; n++) {
      run_demo(all_demos[n]);
  }
}