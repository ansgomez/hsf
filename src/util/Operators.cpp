#include "Operators.h"

using namespace std;

struct timespec operator*(double a, struct timespec b) {
  struct timespec *x = (struct timespec*) malloc(sizeof(struct timespec));
  
  if(a < 0) {
     x->tv_sec = a * b.tv_sec;
     x->tv_nsec = -1*a * b.tv_nsec;  
     
     if(b.tv_sec == 0) {
        x->tv_nsec = -1 * x->tv_nsec;
     }
  }
  else {
     x->tv_sec = a * b.tv_sec;
     x->tv_nsec = a * b.tv_nsec;
  }
  
  if(x->tv_nsec > 999999999) {
     x->tv_sec ++;
     x->tv_nsec = x->tv_nsec - 1000000000;
  }
  
  return *x;
}

struct timespec operator+(struct timespec a, struct timespec b) {
  struct timespec *x = (struct timespec*) malloc(sizeof(struct timespec));
  x->tv_sec = a.tv_sec + b.tv_sec;
  x->tv_nsec = a.tv_nsec + b.tv_nsec;

  if(x->tv_nsec > 999999999) {
    x->tv_sec ++;
    x->tv_nsec = x->tv_nsec - 1000000000;
  }
  else if (x->tv_nsec < 0) {
    x->tv_sec --;
    x->tv_nsec = x->tv_nsec + 1000000000;
  }

  return *x;
}


struct timespec operator-(struct timespec a, struct timespec b) {
  struct timespec *x = (struct timespec*) malloc(sizeof(struct timespec));
  x->tv_sec = a.tv_sec - b.tv_sec;

  if (  a.tv_nsec <  b.tv_nsec && x->tv_sec > 0) {
    x->tv_sec = x->tv_sec-1;
    x->tv_nsec = a.tv_nsec - b.tv_nsec + 1000000000; 
  }
  else {
    x->tv_nsec = a.tv_nsec - b.tv_nsec;
  }

  return *x;
}



int operator < (struct timespec a, struct timespec b) {
  if (a.tv_sec < b.tv_sec) {
    return (1);
  } else {
    if (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec) {
      return (1);
    }
    else {
      return (0);
    }
  }
}

int operator > (struct timespec a, struct timespec b) {
  if (a.tv_sec > b.tv_sec) {
    return (1);
  } else {
    if (a.tv_sec == b.tv_sec && a.tv_nsec > b.tv_nsec) {
      return (1);
    }
    else {
      return (0);
    }
  }
}
