#include "UI.h"
#include "GateIn.h"
#include "GateOut.h"
#include "PotIn.h"
#include "CVIn.h"
#include "pins.h"

UI::UI() {
  // Triggers
  this->trigInA = new GateIn(PIN_TRIG_IN_A, PIN_TRIG_IN_A_SENSE);
  this->clockInA = new GateIn(PIN_CLK_IN_A, PIN_CLK_IN_A_SENSE);
  this->resetInA = new GateIn(PIN_RESET_IN_A, PIN_RESET_IN_A_SENSE);

  this->trigInB = new GateIn(PIN_TRIG_IN_B, PIN_TRIG_IN_B_SENSE);
  this->clockInB = new GateIn(PIN_CLK_IN_B, PIN_CLK_IN_B_SENSE);
  this->resetInB = new GateIn(PIN_RESET_IN_B, PIN_RESET_IN_B_SENSE);

  // Pots
  this->potInRateA = new PotIn(PIN_RATE_POT_A);
  this->potInMorphA = new PotIn(PIN_MORPH_POT_A);
  this->potInChunksA = new PotIn(PIN_CHUNK_POT_A);

  this->potInRateB = new PotIn(PIN_RATE_POT_B);
  this->potInMorphB = new PotIn(PIN_MORPH_POT_B);
  this->potInChunksB = new PotIn(PIN_CHUNK_POT_B);
  
  // CV in
  this->cvInRateA = new CVIn(PIN_RATE_CV_A);
  this->cvInMorphA = new CVIn(PIN_MORPH_CV_A);
  this->cvInChunksA = new CVIn(PIN_CHUNK_CV_A);

  this->cvInRateB = new CVIn(PIN_RATE_CV_B);
  this->cvInMorphB = new CVIn(PIN_MORPH_CV_B);
  this->cvInChunksB = new CVIn(PIN_CHUNK_CV_B);

  // EOC
  this->eocA = new GateOut(PIN_EOC_A);
  this->eocB = new GateOut(PIN_EOC_B);

  // CV Out
  this->cvOutA = new CVOut(PIN_DAC_OUT_A);
  this->cvOutB = new CVOut(PIN_DAC_OUT_B);
}

void UI::init(void) {
  // Triggers
  this->trigInA->init();
  this->clockInA->init();
  this->resetInA->init();

  this->trigInB->init();
  this->clockInB->init();
  this->resetInB->init();

  // Pots
  this->potInRateA->init();
  this->potInMorphA->init();
  this->potInChunksA->init();

  this->potInRateB->init();
  this->potInMorphB->init();
  this->potInChunksB->init();
  
  // CV in
  this->cvInRateA->init();
  this->cvInMorphA->init();
  this->cvInChunksA->init();

  this->cvInRateB->init();
  this->cvInMorphB->init();
  this->cvInChunksB->init();

  // EOC
  this->eocA->init();
  this->eocB->init();

  // CV Out
  this->cvOutA->init();
  this->cvOutB->init();
}

// get the everything from external hardware
void UI::scan(void) {
  // Triggers
  this->trigInA->scan();
  this->clockInA->scan();
  this->resetInA->scan();

  this->trigInB->scan();
  this->clockInB->scan();
  this->resetInB->scan();

  // Pots
  this->potInRateA->scan();
  this->potInMorphA->scan();
  this->potInChunksA->scan();

  this->potInRateB->scan();
  this->potInMorphB->scan();
  this->potInChunksB->scan();
  
  // CV in
  this->cvInRateA->scan();
  this->cvInMorphA->scan();
  this->cvInChunksA->scan();

  this->cvInRateB->scan();
  this->cvInMorphB->scan();
  this->cvInChunksB->scan();
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