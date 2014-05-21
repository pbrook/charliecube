#ifndef CCDEMO_H
#define CCDEMO_H

#include <Arduino.h>
#include "charliecube.h"

#define HZ 25

extern CharlieCubeBase &cube;

class CCDemo
{
public:
  // Return number of ms between frames
  virtual int reset(void) = 0;
  // Return true to advance to the next sequence
  virtual bool tick(void) = 0;
};

void register_demo(CCDemo *demo);

#define DEF_DEMO(name) \
name o_##name; \
void register_##name(void) \
{ \
  register_demo(&o_##name); \
};

#endif
