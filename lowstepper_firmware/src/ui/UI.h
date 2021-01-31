#ifndef UI_H
#define UI_H

#include <Arduino.h>

#include "GateIn.h"
#include "PotIn.h"
#include "CVIn.h"
#include "TrigOut.h"

#define PIN_DAC1 A22
#define PIN_DAC0 A21
#define PIN_TRIG_IN A20
#define PIN_CLOCK_IN A19
#define BLOCK_THIS_PIN_TEMPORARILY A16
#define PIN_RATE_POT A9
#define PIN_SEGMENT_DIVIDE_POT A8
#define PIN_MORPH_POT A7
#define PIN_RATE_CV 0
#define PIN_SEGMENT_DIVIDE_CV 0
#define PIN_MORPH_CV A6

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