#ifndef UI_H
#define UI_H

#include <Arduino.h>

#include "GateIn.h"
#include "PotIn.h"
#include "CVIn.h"
#include "TrigOut.h"

#define PIN_DAC1 A22
#define PIN_DAC0 A21
#define PIN_TRIG_IN_A A20
#define PIN_CLOCK_IN_A A19
#define PIN_RATE_POT_A A9
#define PIN_SEGMENT_DIVIDE_POT_A A8
#define PIN_MORPH_POT_A A7
#define PIN_RATE_CV_A 0
#define PIN_SEGMENT_DIVIDE_CV_A 0
#define PIN_MORPH_CV_A A6

class UI
{
public:
  UI(void);

  void init(void);
  void scan(void);
  int combineAnalogInputs(int, int);

  GateIn* trigInA;
  GateIn* trigInB;

  GateIn* clockInA;
  GateIn* clockInB;

  PotIn* potInRateA;
  PotIn* potInRateB;

  PotIn* potInSegmentDivideA;
  PotIn* potInSegmentDivideB;

  PotIn* potInMorphA;
  PotIn* potInMorphB;

  CVIn* cvInRateA;
  CVIn* cvInRateB;

  CVIn* cvInSegmentDivideA;
  CVIn* cvInSegmentDivideB;

  CVIn* cvInMorphA;
  CVIn* cvInMorphB;

  TrigOut* eocA;
  TrigOut* eocB;
};

#endif