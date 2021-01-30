#include "ui.h"

UI::UI() {
  this->trigIn = new GateIn(PIN_TRIG_IN);
  this->clockIn = new GateIn(PIN_CLOCK_IN);
  this->potInRate = new PotIn(PIN_RATE_POT);
  this->potInSegmentDivide = new PotIn(PIN_SEGMENT_DIVIDE_POT);
  this->potInMorph = new PotIn(PIN_MORPH_POT);
  this->cvInRate = new CVIn(PIN_RATE_POT);
  this->cvInSegmentDivide = new CVIn(PIN_SEGMENT_DIVIDE_CV);
  this->cvInMorph = new CVIn(PIN_MORPH_CV);
  this->eoc1 = new TrigOut(0);
  this->eoc2 = new TrigOut(0);
}

void UI::init(void) {
  trigIn->init();
  clockIn->init();
  potInRate->init();
  potInSegmentDivide->init();
  potInMorph->init();
  cvInRate->init();
  cvInSegmentDivide->init();
  cvInMorph->init();
}

// get the everything from external hardware
void UI::scan(void) {
  trigIn->scan();
  clockIn->scan();
  potInRate->scan();
  potInSegmentDivide->scan();
  potInMorph->scan();
  cvInRate->scan();
  cvInSegmentDivide->scan();
  cvInMorph->scan();
}

int UI::combineAnalogInputs(int potInput, int cvInput)
{
  int value = potInput + ((cvInput * 2) - 1024);

  if (value >= 1023) {
    return 1023;
  }

  if(value <= 1) {
    return 1;
  }

  return value;
}