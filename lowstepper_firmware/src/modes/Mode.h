#ifndef MODES_H
#define MODES_H

#include <cmath>
#include <stdio.h>

class TrigWriter
{
public:
  void setHighForDuration(uint32_t currentTime, uint32_t duration);
  void sendTrig(uint32_t currentTime);
};

class Mode
{
public:
  float tick(
      double frequency,
      double morph,
      int divisions,
      bool trigHigh,
      double inputPhase,
      uint32_t lastTickTime);
  virtual uint32_t getTime(void);
  virtual void writeToDAC(int value);
};

#endif