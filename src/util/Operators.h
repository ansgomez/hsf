#ifndef _OPERATORS_H
#define _OPERATORS_H

#include <time.h>

///This operator multiplies a float with a timespec
struct timespec operator*(double a, struct timespec b);

///This operator adds two timespecs
struct timespec operator+(struct timespec a, struct timespec b);

///This operator subtracts two timespecs
struct timespec operator-(struct timespec a, struct timespec b);

///This operator compares two timespecs 
int operator>(struct timespec a, struct timespec b);

///This operator compares two timespecs
int operator<(struct timespec a, struct timespec b);

#endif
