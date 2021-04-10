#ifndef CVOUT_H
#define CVOUT_H

#include <cmath>

class CVOut
{
private:
  int pin;
public:
  CVOut(int pinNum);
  ~CVOut();

  void init(void);
  void write(uint32_t value);
};

#endif