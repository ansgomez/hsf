#ifndef _OPERATORS_H
#define _OPERATORS_H

struct timespec operator+(struct timespec a, struct timespec b);

struct timespec operator-(struct timespec a, struct timespec b);

int operator> (struct timespec a, struct timespec b);

int operator< (struct timespec a, struct timespec b);


#endif
