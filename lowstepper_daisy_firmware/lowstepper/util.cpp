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

#pragma GCC push_options
#pragma GCC optimize ("O0")
float combinePotAndCv(float potValue, float cvValue) {
	float adjustedCv = mapFFFF(fmin(fmax(cvValue, 0), 0.86f), 0, 0.86f, -1, 1);
	float adjustedPotValue = mapFFFF(
		fmin(potValue, 1.0f),
		0, 1.0f, 0, 1.0f); // TODO: update 0.875 to 1 when CV inputs fixed

	float combinedValue = adjustedPotValue + adjustedCv;
	// float combinedValue = adjustedPotValue;

  return fmin(fmax(combinedValue, 0), 1);
}
#pragma GCC pop_options


float combinePotAndCv2(float potValue, float cvValue) {
	return combinePotAndCv(potValue, cvValue);
}