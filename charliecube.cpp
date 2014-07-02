/*
  CharlieCube
  Copyright (C) Paul Brook <paul@nowt.org>
  Released under the terms of the GNU General Public License version 3

  4x4x4 charlieplexed LED cube driver
 */
 
#include <Arduino.h>
#include <TimerOne.h>
#include "charliecube.h"

// pin mappings are as follows:
// Arduino pin:  3  2  0  1  4  9 10  6 A3 A2 A1 A0
// AVR pin:	D0 D1 D2 D3 D4 B5 B6 D7 F4 F5 F6 F7

#define PD_SHIFT 0
#define PD_MASK 0x9f
#define PB_SHIFT 0
#define PB_MASK 0x60
#define PF_SHIFT 4
#define PF_MASK 0xf0

#define NUM_PIXELS (4*4*4)

static volatile uint16_t *volatile display_base;
static volatile uint16_t *volatile new_display_base;

// This table translates from logical pixel locations to the
// anode/cathode matrix used by the framebuffer
static const uint8_t pixel_map[NUM_PIXELS] PROGMEM = {
#define PIXEL(a, k) ((a) | ((k) << 4))
#include "util/board.h"
};

static void
tick(void)
{
  volatile uint16_t *p;
  static uint8_t cathode;
  uint8_t i;
  uint16_t val;
  uint16_t cathode_mask;

  if (display_base != new_display_base) {
      display_base = new_display_base;
  }
  if (!display_base)
    return;

  cathode_mask = 1u << cathode;
  p = &display_base[cathode * 16];
  /* tristate everything.  */
  DDRD = 0;
  DDRB = 0;
  DDRF = 0;
  for (i = 0; i < 16; i++) {
      val = *(p++);
      PORTD = (val >> PD_SHIFT) & PD_MASK;
      PORTB = (val >> PB_SHIFT) & PB_MASK;
      PORTF = (val >> PF_SHIFT) & PF_MASK;
      val |= cathode_mask;
      DDRD = (val >> PD_SHIFT) & PD_MASK;
      DDRB = (val >> PB_SHIFT) & PB_MASK;
      DDRF = (val >> PF_SHIFT) & PF_MASK;
  }
  cathode++;
  if (cathode == 12)
    cathode = 0;
}

void
CharlieCubeBase::set_pixel(uint8_t x, uint8_t y, uint8_t z, uint8_t bright)
{
  uint8_t pixel_pos;
  uint8_t pos;
  uint16_t mask;
  volatile uint16_t *p;
  uint8_t i;

  // Wrap coordinates
  x &= 3;
  y &= 3;
  z &= 3;
  // We only implement 4 bits of intensity
  bright >>= 4;
  pixel_pos = x + y * 4 + z * 16;
  pos = pgm_read_byte(&pixel_map[pixel_pos]);
  p = &write_base[pos & 0xf0];
  mask = 1u << (pos & 0xf);
  for (i = 0; i < bright; i++)
    *(p++) |= mask;
  mask = ~mask;
  for (; i < 16; i++)
    *(p++) &= mask;
}

void
CharlieCubeBase::clear(void)
{
  uint8_t i;

  for (i = 0; i < 12 * 16; i++) {
      write_base[i] = 0;
  }
}

void
CharlieCubeBase::begin(void)
{
  // 1kHz base frequency gives 83Hz refresh rate
  Timer1.initialize(1000);
  Timer1.attachInterrupt(tick);
}


CharlieCube::CharlieCube(void) : CharlieCubeBase()
{
  write_base = framebuffer;
  new_display_base = framebuffer;
}


void
CharlieCube2::update_base(void)
{
  uint8_t tmp = SREG;
  cli();
  new_display_base = &framebuffer[12*16*display_frame];
  SREG = tmp;
  write_base = &framebuffer[12*16*write_frame];
}

CharlieCube2::CharlieCube2(void) : CharlieCubeBase()
{
  display_frame = 0;
  write_frame = 1;
  update_base();
}

void
CharlieCube2::swap(void)
{
  write_frame = !write_frame;
  if (write_frame == display_frame)
    display_frame = !display_frame;
  update_base();
}

void
CharlieCube2::wait(void)
{
  while (new_display_base != display_base)
    /* no-op */ ;
}

void
CharlieCube2::single_buffer(void)
{
  write_frame = display_frame;
  update_base();
}
