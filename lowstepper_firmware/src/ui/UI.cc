#include "ui.h"

UI::UI()
{
  this->trigIn = new GateIn(PIN_TRIG_IN);
  this->clockIn = new GateIn(PIN_CLOCK_IN);
  this->potInRate = new PotIn(PIN_RATE_POT);
  this->potInSegmentDivide = new PotIn(PIN_SEGMENT_DIVIDE_POT);
  this->potInMorph = new PotIn(PIN_MORPH_POT);
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
