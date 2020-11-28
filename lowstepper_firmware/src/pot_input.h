#ifndef POTS_IN_H
#define POTS_IN_H

class PotInput 
{
private:
  int pin;
  double value;
public:
  PotInput(int pinNum);
  ~PotInput();

  void init(void);
  void scan(void);
  double getValue();
};

#endif