#ifndef _TimeSpec_H
#define _TimeSpec_H

#include <time.h>

class TimeSpec {
 public:

  unsigned long int seconds;
  unsigned long int millis;
  unsigned long int micros;
  unsigned long int nanos;

  struct timespec getTimespec();

  int operator +(TimeSpec ts);
  int operator -(TimeSpec ts);
  int operator >(TimeSpec ts);
  int operator <(TimeSpec ts);
};

#endif
