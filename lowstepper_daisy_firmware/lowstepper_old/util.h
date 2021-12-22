#ifndef UTIL_H 
#define UTIL_H

uint16_t map(float x, float in_min, float in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double roundTenth(double v) {
  int c, r, m;
  m = v*100;
  c = m%10;
  r = m/10;
  if(c>=5)
  r++;
  return (double)r/10;
}

double plusMinusOneToAnalogOut(double n) {
  return (n * 2000.0) + 2050.0;
}

#endif