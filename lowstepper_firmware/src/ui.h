#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include "gate_in.h"
#include "pot_input.h"

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
  PotInput* potInRate;
  PotInput* potInSegmentDivide;
  PotInput* potInMorph;
};

UI::UI()
{
  this->trigIn = new GateIn(PIN_TRIG_IN);
  this->clockIn = new GateIn(PIN_CLOCK_IN);
  this->potInRate = new PotInput(PIN_RATE_POT);
  this->potInSegmentDivide = new PotInput(PIN_SEGMENT_DIVIDE_POT);
  this->potInMorph = new PotInput(PIN_MORPH_POT);
}

void UI::init(void) {
  trigIn->init();
  clockIn->init();
  potInRate->init();
  potInSegmentDivide->init();
  potInMorph->init();
}

void UI::scan(void)
{
  trigIn->scan();
  clockIn->scan();
  potInRate->scan();
  potInSegmentDivide->scan();
  potInMorph->scan();
}

#endif