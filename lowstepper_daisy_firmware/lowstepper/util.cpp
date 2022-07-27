#include <cmath>
#include "util.h"

float mapFFFF(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t mapFFII(float x, float in_min, float in_max, int out_min, int out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// potValue = 0 -> 1
// cvValue = 0 -> 1
// cvValue always at 0.5 when unplugged
// clamps value between 0 and 1
float combinePotAndCv(float potValue, float cvValue) {
	float adjustedCv = mapFFFF(cvValue, 0, 1, -1, 1);
	float combinedValue = potValue + adjustedCv;
  return fmin(fmax(combinedValue, 0), 1);
}
