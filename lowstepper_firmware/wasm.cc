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
  // STUB
  //===========================================================

  class TimeProviderStub : public TimeProvider
  {
  public:
    virtual uint32_t getTime(void);
  };

  uint32_t TimeProviderStub::getTime()
  {
    return globalTime;
  }

  class TrigWriterStub : public TrigWriter
  {
  public:
    virtual void setHighForDuration(uint32_t currentTime, uint32_t duration);
    virtual void sendTrig(uint32_t currentTime);
  };
  void TrigWriterStub::setHighForDuration(uint32_t currentTime, uint32_t duration) {}
  void TrigWriterStub::sendTrig(uint32_t currentTime) {}

  class DACWriterStub : public DACWriter
  {
  public:
    virtual void write(uint32_t value);
  };

  void
  DACWriterStub::write(uint32_t value)
  {
    // Do nothing.
  }

  TimeProviderStub *timeProvider = new TimeProviderStub();
  TrigWriterStub *trigWriter = new TrigWriterStub();
  DACWriterStub *dacWriter = new DACWriterStub();

  int MODE_COUNT = 2;
  int modeIndex = 0;

  SteppedLfoTrig t1 = SteppedLfoTrig{timeProvider, trigWriter, dacWriter};
  SteppedLfoTrig t2 = SteppedLfoTrig{timeProvider, trigWriter, dacWriter};
  Mode<BaseMode *> mode1 = Mode<BaseMode *>{&t1, &t2};

  SteppedLfoGate g1 = SteppedLfoGate{timeProvider, trigWriter, dacWriter};
  SteppedLfoGate g2 = SteppedLfoGate{timeProvider, trigWriter, dacWriter};
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
        mode->phase,
        mode->lastMicros);
    return f;
  }

#ifdef __cplusplus
}
#endif