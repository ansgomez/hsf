#ifndef _OPERATORS_H
#define _OPERATORS_H

struct timespec operator+(struct timespec a, struct timespec b);

struct timespec operator-(struct timespec a, struct timespec b);

int operator> (struct timespec a, struct timespec b);

int operator< (struct timespec a, struct timespec b);

///This function returns a timespec with the specified seconds
struct timespec Seconds(unsigned int s);

///This function returns a timespec with the specified milliseconds
struct timespec Millis(unsigned int ms);

///This function returns a timespec with the specified microseconds
struct timespec Micros(unsigned int us);

#endif
