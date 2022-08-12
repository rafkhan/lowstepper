#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

#include "AverageBuffer.h"
#include <cstddef>

class SyncManager
{
  private:
    float sampleRate;
    bool useSync;
    int samplesSinceLastSyncTick = 1;
    AverageBuffer<float> *bpmAverage;
  public:
    SyncManager();
    void init(float sampleRate);
    bool tick(bool isGateHigh);
    float getBpm(void);
};


#endif