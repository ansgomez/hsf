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
