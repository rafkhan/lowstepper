#ifndef UTIL_H 
#define UTIL_H

#include <cstdint>

#ifndef TWO_PI
#define TWO_PI 6.28318530718f
#endif

uint16_t mapFFII(float x, float in_min, float in_max, int out_min, int out_max);
float mapFFFF(float x, float in_min, float in_max, float out_min, float out_max);
float combinePotAndCv(float potValue, float cvValue);

#endif