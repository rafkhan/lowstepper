#ifndef LFO_H
#define LFO_H

#include <cmath>

const int INTERPOLATE_MAX = 1000;
const int INTERPOLATE_MIN = 1;

float pi = 3.14159;
float twopi = pi * 2;

float triangle(float phase) {
  float unadjustedAmp = std::abs(std::fmod((phase - pi/2), twopi) - pi);
  return unadjustedAmp * 2 -1;
}

float saw(float phase) {
  return -1 * ((phase - pi) / pi);
}

float square(float phase) {
  if(phase >= pi) { return -1; }
  return 1;
}

#endif