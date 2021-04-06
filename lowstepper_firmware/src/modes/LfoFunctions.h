#ifndef LFO_FUNCTIONS_H 
#define LFO_FUNCTIONS_H

#include "../util.h"

#define PI 3.14159265359
#define TWO_PI 6.28318530718

/**
 * All of the following functions take a phase input from 0 - TWO_PI
 * and return a value from -1 to 1.
 * 
 * std::sin is also leveraged but not included because sine waves are
 * incredibly based and just do that by default
 * 
 * lmao math
 */
double triangle(double phase)
{
  //REALLY DUMB CODE, NEED TO BE OPTIMIZED!

  if (phase <= PI * 0.5) //FIRST QUARTER
  {
    return 2 + ( -1 * (((PI - phase * 2) + PI) / PI) ); //GO DOWN!
  }

  if(phase >= PI * 1.5) //FOURTH QUARTER
  {
    return -1 + ( -1 * (((TWO_PI - phase * 2) + PI) / PI) ) ; //GO DOWN TOO!
  } 
  
  //SECOND AND THIRD QUARTER 
  return 1 + ( -1 * ((phase*2 - PI) / PI)); //GO UP!

}

double ramp(double phase)
{
  return -1 * ((phase - PI) / PI);
}

double saw(double phase) {
  if (phase <= PI) {
    return phase / PI;
  }

  return ((phase - PI) / PI) - 1;
}

double square(double phase)
{
  if (phase >= PI)
  {
    return -1;
  }
  return 1;
}

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

// TODO: stop computing the same thing more than once!!!
// TODO: CLEAN UP THIS LOGIC.
double getMorphedOutput(double morphPosition, double phase)
{
  double newMorph;
  if(morphPosition <= 0.33) {
    newMorph = map(morphPosition, 0, 0.33, 0, 1);
    return sin(phase) + (newMorph * (triangle(phase) - sin(phase)));
  } else if(morphPosition > 0.33 && morphPosition <= 0.66) {
    newMorph = map(morphPosition, 0.34, 0.66, 0, 1);
    return triangle(phase) + (newMorph * (saw(phase) - triangle(phase)));
  } else {
    newMorph = map(morphPosition, 0.67, 1, 0, 1);
    return saw(phase) + (newMorph * (square(phase) - saw(phase)));
  }
}

#endif