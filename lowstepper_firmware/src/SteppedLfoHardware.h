#ifndef STEPPED_LFO_HARDWARE
#define STEPPED_LFO_HARDWARE

#include "SteppedLfo.h"
#include "util.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include "ui/ui_emscripten.h"
#else
#include "ui/ui.h"
#endif

class SteppedLfoHardware: public SteppedLfo {
  public:
    SteppedLfoHardware(int pin);
    uint32_t getTime(void);
    void writeToDAC(int value);
  private:
    int pin;

    void setStateFromExternalInputs(UI ui);
};

SteppedLfoHardware::SteppedLfoHardware(int pin) : SteppedLfo()
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT); // TODO abstract this
}

uint32_t SteppedLfoHardware::getTime(void)
{
  return micros();
}

void SteppedLfoHardware::writeToDAC(int value)
{
  analogWrite(this->pin, value);
}

void SteppedLfoHardware::setStateFromExternalInputs(UI ui)
{
  clockInBpm = roundTenth(calculateBpm(ui));

  int potRate = map(ui.potInRate->getValue(), 1, 1023, 1, 5);
  lfoFreq = clockInBpm / (15 * pow(2, potRate));

  // if(potRate == 1) {
  //   Serial.println("1/4");
  // } else if(potRate == 2) {
  //   Serial.println("1/2");
  // } else if(potRate == 3) {
  //   Serial.println("1");
  // } else if(potRate == 4) {
  //   Serial.println("2");
  // } else if(potRate == 5) {
  //   Serial.println("4");
  // }

  divisions = map(ui.potInSegmentDivide->getValue(), 1, 1023, 1, 8);

  int morphInput = ui.combineAnalogInputs(
    ui.potInMorph->getValue(),
    ui.cvInMorph->getValue()
  );
  morph = map((double) morphInput, 1, 1023, 0, 1);
}

#endif