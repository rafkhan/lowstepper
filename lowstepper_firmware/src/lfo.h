#ifndef LFO_H
#define LFO_H

#include <cmath>

const int INTERPOLATE_MAX = 1000;
const int INTERPOLATE_MIN = 1;

#define PI 3.14159
#define TWO_PI 6.28318

/**
 * All of the following functions take a phase input from 0 - TWO_PI
 * and return a value from 0 - 1.
 * 
 * std::sin is also leveraged but not included because sine waves are
 * incredibly based and just do that by default
 * 
 * lmao math
 */
double triangle(double phase) {
  double unadjustedAmp = std::abs(std::fmod((phase - PI/2), TWO_PI) - PI);
  return unadjustedAmp * 2 -1;
}

double saw(double phase) {
  return -1 * ((phase - PI) / PI);
}

double square(double phase) {
  if(phase >= PI) { return -1; }
  return 1;
}

/**
 * Note to self: read your damn whiteboard, but write it here anyway
 * 
 * w = value between 0 and 1, weight to morph between functions f and g
 * x = phase between 0 and 2pi
 * f = function that is returned when w = 0
 * g = function that is returned when w = 1
 * 
 * I(w, x, f, g) = f(x) + w(g(x) - f(x))
 * I(x) = morphed wave function
 */
double fuckYeahLfo(double morph, double phase) {
  double f, g;

  // only sin / sq for now
  return sin(phase) + morph * (square(phase) - sin(phase));
}

#endif