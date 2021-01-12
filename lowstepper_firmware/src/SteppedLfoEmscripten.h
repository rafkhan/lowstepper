#ifndef STEPPED_LFO_EMSCRIPTEN
#define STEPPED_LFO_EMSCRIPTEN

#include "SteppedLfo.h"

class SteppedLfoEmscripten : public SteppedLfo {
  public:
    uint32_t getTime(void);
    void setTime(uint32_t t);
  private:
    uint32_t time;
};

void SteppedLfoEmscripten::setTime(uint32_t t)
{
  this->time = t;
}

uint32_t SteppedLfoEmscripten::getTime(void)
{
  return this->time;
}

#endif