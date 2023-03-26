#include <cmath>
#include <math.h>
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

float combinePotAndCv(float potValue, float cvValue, bool applyThreshold) {
	float adjustedCv = mapFFFF(fmin(fmax(cvValue, 0), 0.855f), 0, 0.855f, -1, 1);
	if(applyThreshold && 
		((adjustedCv > 0 && adjustedCv < 0.024) || (adjustedCv < 0 && adjustedCv > -0.024))
	) {
		adjustedCv = 0;
	}

	float adjustedPotValue = mapFFFF(
		fmin(potValue, 1.0f),
		0, 1.0f, 0, 1.0f);

	float combinedValue = adjustedPotValue + adjustedCv;

  return fmin(fmax(combinedValue, 0), 1);
}