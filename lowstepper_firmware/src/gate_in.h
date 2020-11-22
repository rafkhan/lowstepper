#ifndef GATE_IN_H
#define GATE_IN_H

class GateIn 
{
private:
  int pin;
  bool isPinLow;
  bool isGateHigh;
public:
  GateIn(int pinNum);
  ~GateIn();

  void init(void);
  void scan(void);
  bool checkGateHigh(void);
};

#endif