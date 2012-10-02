#ifndef _OPERATORS_H
#define _OPERATORS_H

#include <time.h>

struct timespec operator+(struct timespec a, struct timespec b);

struct timespec operator-(struct timespec a, struct timespec b);

int operator>(struct timespec a, struct timespec b);

int operator<(struct timespec a, struct timespec b);

///This function returns a timespec with the specified seconds
struct timespec Seconds(__time_t s);

///This function returns a timespec with the specified milliseconds
struct timespec Millis(long int ms);

///This function returns a timespec with the specified microseconds
struct timespec Micros(long int us);

#endif
