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

CharlieCube real_cube;

CharlieCubeBase &cube = real_cube;

void setup()
{
  Serial.begin(9600);
  // Not great, but better than nothing
  randomSeed(analogRead(0));
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
  long expires;

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
  }
}

void loop()
{
  int n;

  for (n = 0; n < num_demos; n++) {
      run_demo(all_demos[n]);
  }
}
