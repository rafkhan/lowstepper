#ifndef STEPPED_LFO_HARDWARE
#define STEPPED_LFO_HARDWARE

#include "SteppedLfo.h"

class SteppedLfoHardware: public SteppedLfo {
  public:
    SteppedLfoHardware(int pin);
    uint32_t getTime(void);
    void writeToDAC(int value);
  private:
    int pin;
};

SteppedLfoHardware::SteppedLfoHardware(int pin)
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT); // TODO abstract this
}

uint32_t SteppedLfoHardware::getTime(void)
{
  return micros();
}

void SteppedLfoHardware::writeToDAC(int value)
{
  analogWrite(this->pin, value);
}

#endif