#include "SyncManager.h"
#include "AverageBuffer.h"
#include <limits.h>

SyncManager::SyncManager() {}

bool SyncManager::tick(bool isGateHigh) {
  if(isGateHigh) {
    float bpm = 60.0f / ((samplesSinceLastSyncTick + 1.0f) * (1.0f / sampleRate)) / 4;
    bpmAverage->addValue(bpm);
    samplesSinceLastSyncTick = 0;
    useSync = true;
  } else {
    samplesSinceLastSyncTick++;
    samplesSinceLastSyncTick = samplesSinceLastSyncTick % (INT_MAX -  1); // todo check this

    if(samplesSinceLastSyncTick > sampleRate) {
      useSync = false;
    }
  }

  return useSync;
}

void SyncManager::init(float sampleRate) {
  this->sampleRate = sampleRate;
  AverageBuffer<float>* buf = new AverageBuffer<float>((size_t) 4, sampleRate / 120.0f); // Use a reasonable starting BPM
  this->bpmAverage = buf;
}

float SyncManager::getBpm(void) {
  return this->bpmAverage->getAverageValue();
}