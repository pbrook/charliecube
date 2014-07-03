/*
  CharlieCube serial bridge
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3
 */
 
#include <TimerOne.h>
#include <charliecube.h>

CharlieCube2 cube;

void setup()
{
  Serial.begin(9600);
  cube.begin();
  cube.single_buffer();
}

static uint8_t
read_byte(void)
{
  while (!Serial.available())
    /* no-op */;
  return Serial.read();
}

typedef enum {
    SM_IDLE,
    SM_READY,
    SM_ACTIVE
} serial_mode;

static void
set_frames(uint8_t new_display, uint8_t new_write)
{
  static uint8_t display_frame;
  static uint8_t write_frame;
  if (display_frame == new_display && write_frame == new_write)
    return;
  if (display_frame == new_display) {
      if (new_display == new_write) {
	  cube.single_buffer();
      } else {
	  cube.swap();
      }
  } else {
      if (display_frame == write_frame)
	cube.swap();
      cube.swap();
      if (new_display == new_write)
	cube.single_buffer();
  }
  cube.wait();
  display_frame = new_display;
  write_frame = new_write;
}

void loop()
{
  uint8_t cmd;
  uint8_t d0;
  uint8_t d1;
  uint8_t d2;
  serial_mode sm = SM_IDLE;

  while (true) {
      cmd = read_byte();
      if (cmd == 0xff)
	continue;
      d0 = read_byte();
      d1 = read_byte();
      d2 = read_byte();
      if (cmd == 0xe0) {
	  if (d0 == 0xf0 && d1 == 0xf1 && d2 == 0xf2)
	    sm = SM_READY;
	  else
	    sm = SM_IDLE;
	  continue;
      }
      if (cmd < 0xd0 && sm != SM_ACTIVE) {
	  continue;
      }
      if (sm == SM_IDLE)
	continue;
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
  }
}
