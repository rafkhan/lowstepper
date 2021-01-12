#ifndef ANALOG_IN_IN 
#define ANALOG_IN_IN

class AnalogIn
{
private:
  int pin;
  double value;
public:
  AnalogIn(int pinNum);
  ~AnalogIn();

  void init(void);
  void scan(void);
  double getValue();
};

#endif