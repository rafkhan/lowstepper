#ifndef GATEINPUT_H 
#define GATEINPUT_H

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class GateInput {
  public:
    void init(DaisySeed *hw, int pinGpio, int pinDetect);
    void readHardware(void);
    bool triggerCheck(void);
    bool isCablePluggedIn(void);
    bool isGateHigh(void);

  private:
    dsy_gpio gpio;
    dsy_gpio gpioDetect;
    DaisySeed *hw;

    bool gpioValue = false;
    bool gpioDetectValue = false;

    bool lastValueHigh = false;
    bool trigger = false;
};

#endif