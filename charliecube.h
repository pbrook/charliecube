class CharlieCubeBase
{
protected:
  volatile uint16_t *write_base;
public:
  CharlieCubeBase(void) {};
  void set_pixel(uint8_t x, uint8_t y, uint8_t z, uint8_t val);
  void begin(void);
};

class CharlieCube : public CharlieCubeBase
{
  volatile uint16_t framebuffer[12*16];
public:
  CharlieCube(void);
};

