#include <stdio.h>
#include <emscripten/emscripten.h>
#include <cmath>
#include "src/modes/SteppedLfoGate.h"
#include "src/modes/SteppedLfoTrig.h"
#include "src/modes/BaseMode.h"
#include "src/modes/Mode.h"

#define __EMSCRIPTEN__ 1

int main(int argc, char *argv[])
{
  printf("printing to web console");
}

#ifdef __cplusplus
extern "C"
{
#endif

  volatile uint32_t globalTime = 0;

  volatile int bpmA = 0;
  volatile double rateA = 512;
  volatile double morphA = 0;
  volatile int chunksA = 1;
  volatile bool shouldUseTriggerA = true;
  volatile bool trigHighA = false;
  volatile bool didSetTrigA = false;
  volatile bool shouldUseClockA = false;

  volatile int bpmB = 0;
  volatile double rateB = 512;
  volatile double morphB = 0;
  volatile int chunksB = 1;
  volatile bool shouldUseTriggerB = true;
  volatile bool trigHighB = false;
  volatile bool didSetTrigB = false;
  volatile bool shouldUseClockB = false;

  //===========================================================
  // LFO STUB
  //===========================================================

  class SteppedLfoTrigEmscripten : public SteppedLfoTrig
  {
  public:
    void writeToDAC(int value);
    uint32_t getTime(void);
  };

  uint32_t SteppedLfoTrigEmscripten::getTime() { return globalTime; }
  void SteppedLfoTrigEmscripten::writeToDAC(int value)
  { /* do nothing :) */
  }

  class SteppedLfoGateEmscripten : public SteppedLfoGate
  {
  public:
    void writeToDAC(int value);
    uint32_t getTime(void);
  };

  uint32_t SteppedLfoGateEmscripten::getTime() { return globalTime; }
  void SteppedLfoGateEmscripten::writeToDAC(int value)
  { /* do nothing :) */
  }

  //===========================================================
  // TW STUB
  //===========================================================
  class TrigWriterStub : public TrigWriter
  {
  public:
    void setHighForDuration(uint32_t currentTime, uint32_t duration);
    void sendTrig(uint32_t currentTime);
  };

  void TrigWriterStub::setHighForDuration(uint32_t currentTime, uint32_t duration) {}
  void TrigWriterStub::sendTrig(uint32_t currentTime) {}

  TrigWriterStub tw;

  int MODE_COUNT = 2;
  int modeIndex = 0;

  SteppedLfoTrigEmscripten t1 = SteppedLfoTrigEmscripten{};
  SteppedLfoTrigEmscripten t2 = SteppedLfoTrigEmscripten{};
  Mode<BaseMode *> mode1 = Mode<BaseMode *>{&t1, &t2};

  SteppedLfoGateEmscripten g1 = SteppedLfoGateEmscripten{};
  SteppedLfoGateEmscripten g2 = SteppedLfoGateEmscripten{};
  Mode<BaseMode *> mode2 = Mode<BaseMode *>{&g1, &g2};

  Mode<BaseMode *> modeList[] = {mode1, mode2};

  EMSCRIPTEN_KEEPALIVE void setGlobalTime(uint32_t t)
  {
    globalTime = t;
  }

  EMSCRIPTEN_KEEPALIVE void setBpmA(int bpm)
  {
    bpmA = bpm;
  }

  EMSCRIPTEN_KEEPALIVE void setGateA(bool g)
  {
    // debounce logic below...
    // has to be low before it can be set high again
    if (!didSetTrigA && g)
    {
      trigHighA = true;
      didSetTrigA = true;
    }

    if (didSetTrigA && !g)
    {
      didSetTrigA = false;
      trigHighA = false;
    }
  }

  EMSCRIPTEN_KEEPALIVE void setRateA(int r)
  {
    rateA = r;
  }

  EMSCRIPTEN_KEEPALIVE void setMorphA(double m)
  {
    morphA = m;
  }

  EMSCRIPTEN_KEEPALIVE void setChunksA(int c)
  {
    chunksA = c;
  }

  EMSCRIPTEN_KEEPALIVE uint32_t getTrigA()
  {
    return trigHighA;
  }

  EMSCRIPTEN_KEEPALIVE float tickLFOA(uint32_t t)
  {
    globalTime = t;
    BaseMode *mode = modeList[modeIndex].handlerA;
    float f = mode->tick(
        rateA,
        morphA,
        chunksA,
        trigHighA,
        &tw,
        mode->phase,
        mode->lastMicros);
    return f;
  }

#ifdef __cplusplus
}
#endif