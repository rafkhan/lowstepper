#ifndef UTIL_H 
#define UTIL_H

double roundTenth(double v) {
  int c, r, m;
  m = v*100;
  c = m%10;
  r = m/10;
  if(c>=5)
  r++;
  return (double)r/10;
}

#endif