#ifndef TRIG_OUT_H
#define TRIG_OUT_H

class TrigOut 
{
private:
  int pin;
  bool isHigh;
  uint32_t startTime;
  uint32_t endTime;

  void setHigh(void);
  void setLow(void);
public:
  TrigOut(int pinNum);
  ~TrigOut();

  void init(void);
  void setHighForDuration(uint32_t currentTime, uint32_t duration);
  void tick(uint32_t currentTime);
};

#endif