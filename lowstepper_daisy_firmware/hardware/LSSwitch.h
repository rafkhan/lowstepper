#ifndef SWITCH_H
#define SWITCH_H

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class LSSwitch {
  public:
    void init(DaisySeed *hw, int pinGpio);
    bool isOn();
    void readHardware(void);
  private:
    dsy_gpio gpio;
    DaisySeed *hw;
    uint8_t gpioValue;
};

#endif