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

	float adjustedCv = mapFFFF(fmin(fmax(cvValue, 0), 0.87f), 0, 0.87f, -1, 1);
	if((adjustedCv > 0 && adjustedCv < 0.008) || (adjustedCv < 0 && adjustedCv > -0.008)) {
		adjustedCv = 0;
	}

	float adjustedPotValue = mapFFFF(
		fmin(potValue, 1.0f),
		0, 1.0f, 0, 1.0f);

	float combinedValue = adjustedPotValue + adjustedCv;

  return fmin(fmax(combinedValue, 0), 1);
}
#pragma GCC pop_options


float combinePotAndCv2(float potValue, float cvValue) {
	return combinePotAndCv(potValue, cvValue);
}