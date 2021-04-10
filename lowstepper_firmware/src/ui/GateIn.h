#ifndef GATE_IN_H
#define GATE_IN_H

class GateIn 
{
private:
  int pin;
  int pinSense;
  bool isPinLow;
  bool isTrigHigh;
  bool cableSensed;
public:
  GateIn(int pinNum, int pinSense);
  ~GateIn();

  void init(void);
  void scan(void);
  bool checkGateHigh(void);
  bool checkTrigHigh(void);
  bool hasCablePluggedIn(void);
};

#endif