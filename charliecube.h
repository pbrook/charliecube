#ifndef CHARLIECUBE_H
#define CHARLIECUBE_H

#include <stdint.h>

// Common base class for single and double buffered implementations.
// Do not use directly.
class CharlieCubeBase
{
protected:
  volatile uint16_t *write_base;
public:
  CharlieCubeBase(void) {};
  // Set a pixel value.  Brightness is from 0 (off) to 255 (full on).
  void set_pixel(uint8_t x, uint8_t y, uint8_t z, uint8_t bright);
  // Start the cube driver
  void begin(void);
};

// Single buffered
class CharlieCube : public CharlieCubeBase
{
  volatile uint16_t framebuffer[12*16];
public:
  CharlieCube(void);
};

// Double buffered
class CharlieCube2 : public CharlieCubeBase
{
  uint8_t display_frame:1;
  uint8_t write_frame:1;
  volatile uint16_t framebuffer[12*16*2];
public:
  CharlieCube2(void);
  // Swap front and back buffers
  void swap(void);
  // Wait for buffer swap to complete
  void wait(void);
  // Send writes directly to front buffer
  // Calling swap() will reset to double buffered mode without changing the displayed image
  void single_buffer(void);
private:
  void update_base(void);
};

#endif
