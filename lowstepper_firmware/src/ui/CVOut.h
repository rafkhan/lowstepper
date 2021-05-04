#ifndef CVOUT_H
#define CVOUT_H

#include <cmath>
#include "../modes/BaseMode.h"

class CVOut : public DACWriter
{
private:
  int pin;
public:
  CVOut(int pinNum);
  ~CVOut();

  void init(void);
  virtual void write(uint32_t value);
};

#endif