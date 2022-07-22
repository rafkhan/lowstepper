#ifndef GATE_IN_H 
#define GATE_IN_H

#include <cstdint>
#include "daisy_seed.h"

class GateInput {
  private:
    dsy_gpio gpio;
    dsy_gpio gpioDetect;
    int pinState; // 1 = 3v3, 0 = 0V
    int pinDetectState; // 1 = 3v3, 0 = 0V
  public:
    void init(daisy::DaisySeed *seed, uint8_t pin, uint8_t pinDetect);
    void tick();
    bool isGateHigh();
    bool isPluggedIn();
};

#endif