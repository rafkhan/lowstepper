#ifndef GATEOUTPUT_H
#define GATEOUTPUT_H

#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

class GateOutput {
  public:
    void init(DaisySeed *hw, float sampleRate, float blockSize, int pinGpio);
    void setGateHigh(int ms);
    void tick(void);

  private:
    dsy_gpio gpio;
    DaisySeed *hw;

    float sampleRate;
    float blockSize;

    uint64_t sampleCount = 0;
    uint64_t sampleCountEnd = 0;
};

#endif