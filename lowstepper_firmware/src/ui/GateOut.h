#ifndef TRIG_OUT_H
#define TRIG_OUT_H

#include <cmath>

class GateOut 
{
private:
  int pin;
  bool isHigh;
  uint32_t startTime;
  uint32_t endTime;

  void setHigh(void);
  void setLow(void);
public:
  GateOut(int pinNum);
  ~GateOut();

  void init(void);
  void setHighForDuration(uint32_t currentTime, uint32_t duration);
  void tick(uint32_t currentTime);
};

#endif