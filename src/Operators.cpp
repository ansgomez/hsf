#include "Operators.h"

#include <stdlib.h>
#include <time.h>

struct timespec operator+(struct timespec a, struct timespec b) {
  struct timespec *x = (struct timespec*) malloc(sizeof(struct timespec));
  x->tv_sec = a.tv_sec + b.tv_sec;
  x->tv_nsec = a.tv_nsec + b.tv_nsec;

  if(x->tv_nsec > 999999999) {
    x->tv_sec ++;
    x->tv_nsec = x->tv_nsec - 1000000000;
  }

  return *x;
}

struct timespec operator-(struct timespec a, struct timespec b) {
  struct timespec *x = (struct timespec*) malloc(sizeof(struct timespec));
  x->tv_sec = a.tv_sec - b.tv_sec;

  if (  a.tv_nsec <  b.tv_nsec) {
    x->tv_sec = x->tv_sec-1;
    x->tv_nsec = a.tv_nsec - b.tv_nsec + 1000000000; 
  }
  else {
    x->tv_nsec = a.tv_nsec - b.tv_nsec;
  }

  return *x;
}

int operator> (struct timespec a, struct timespec b) {
  if (a.tv_sec > b.tv_sec) {
    return (1);
  } else {
    if (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec)
      return (1);
    else
      return (0);
  }
}

int operator< (struct timespec a, struct timespec b) {
  if (a.tv_sec < b.tv_sec) {
    return (1);
  } else {
    if (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec)
      return (1);
    else
      return (0);
  }
}


///This function returns a timespec with the specified seconds
struct timespec Seconds(unsigned int s) {
  struct timespec aux;

  aux.tv_sec = s;
  aux.tv_nsec = 0;

  return aux;
}

///This function returns a timespec with the specified milliseconds
struct timespec Millis(unsigned int ms) {
  struct timespec aux;

  aux.tv_sec = 0;
  aux.tv_nsec = ms*1000000;

  return aux;
}

///This function returns a timespec with the specified microseconds
struct timespec Micros(unsigned int us) {
  struct timespec aux;

  aux.tv_sec = 0;
  aux.tv_nsec = us*1000;

  return aux;
}
