#include "ui.h"

UI::UI() {
  this->trigInA = new GateIn(PIN_TRIG_IN_A);
  this->trigInB = new GateIn(0);

  this->clockInA = new GateIn(PIN_CLOCK_IN_A);
  this->clockInB = new GateIn(0);
  
  this->potInRateA = new PotIn(PIN_RATE_POT_A);
  this->potInRateB = new PotIn(0);

  this->potInSegmentDivideA = new PotIn(PIN_SEGMENT_DIVIDE_POT_A);
  this->potInSegmentDivideB = new PotIn(0);
  
  this->potInMorphA = new PotIn(PIN_MORPH_POT_A);
  this->potInMorphB = new PotIn(0);

  this->cvInRateA = new CVIn(PIN_RATE_POT_A);
  this->cvInRateB = new CVIn(0);

  this->cvInSegmentDivideA = new CVIn(PIN_SEGMENT_DIVIDE_CV_A);
  this->cvInSegmentDivideB = new CVIn(0);

  this->cvInMorphA = new CVIn(PIN_MORPH_CV_A);
  this->cvInMorphB = new CVIn(0);

  this->eocA = new TrigOut(0);
  this->eocB = new TrigOut(0);
}

void UI::init(void) {
  trigInA->init();
  trigInB->init();

  clockInA->init();
  clockInB->init();

  potInRateA->init();
  potInRateB->init();

  potInSegmentDivideA->init();
  potInSegmentDivideB->init();

  potInMorphA->init();
  potInMorphB->init();

  cvInRateA->init();
  cvInRateB->init();

  cvInSegmentDivideA->init();
  cvInSegmentDivideB->init();

  cvInMorphA->init();
  cvInMorphB->init();
}

// get the everything from external hardware
void UI::scan(void) {
  trigInA->scan();
  trigInB->scan();

  clockInA->scan();
  clockInB->scan();

  potInRateA->scan();
  potInRateB->scan();

  potInSegmentDivideA->scan();
  potInSegmentDivideB->scan();

  potInMorphA->scan();
  potInMorphB->scan();

  cvInRateA->scan();
  cvInRateB->scan();

  cvInSegmentDivideA->scan();
  cvInSegmentDivideB->scan();

  cvInMorphA->scan();
  cvInMorphB->scan();
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