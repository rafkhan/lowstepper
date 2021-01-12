#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include "GateIn.h"
#include "PotIn.h"

#define PIN_DAC1 A22
#define PIN_DAC0 A21
#define PIN_TRIG_IN A20
#define PIN_CLOCK_IN A19 // TODO build
#define BLOCK_THIS_PIN_TEMPORARILY A16
#define PIN_RATE_POT A9           // TODO build
#define PIN_SEGMENT_DIVIDE_POT A8 // TODO build
#define PIN_MORPH_POT A7          // TODO build

class UI
{
public:
  UI(void);
  void init(void);
  void scan(void);
  GateIn* trigIn;
  GateIn* clockIn;
  PotIn* potInRate;
  PotIn* potInSegmentDivide;
  PotIn* potInMorph;
};

#endif