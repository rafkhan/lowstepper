#ifndef UTIL_H 
#define UTIL_H

#ifndef TWO_PI
#define TWO_PI 6.28318530718f
#endif

uint16_t mapFFII(float x, float in_min, float in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif