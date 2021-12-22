#include "LowStepperChannel.h"
#include <cstddef>

typedef void (* LSCallback) (LowStepperOutput[], size_t);

class LowStepper {
  public:
    LowStepper(LowStepperChannel *a, LowStepperChannel *b);
    LowStepperOutput* tick();

  private:
    LowStepperChannel* lsA;
    LowStepperChannel* lsB;

    float lastPhaseA = 0.0f;
    float lastPhaseB = 0.0f;
};
