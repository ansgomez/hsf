#ifndef _TIMEUTIL_H
#define _TIMEUTIL_H

#include "util/Enumerations.h"

#include <time.h>
#include <stdlib.h>

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class TimeUtil {
  public:
  ///This variable holds the (absolute) time at which the simulation started (used to calculate relative time)
  static struct timespec offset;

  /*********** MEMBER FUNCTIONS ***********/
  public:
  ///This function returns a timespec with the current time
  static struct timespec getTime();

  ///This function returns a timespec with the current time
  static struct timespec getTime(enum _time_type type);

  //Sets the relative-time offset to current time
  static void setOffset();

  ///This function returns a timespec with the specified seconds
  static struct timespec Seconds(__time_t s);

  ///This function returns a timespec with the specified milliseconds
  static struct timespec Millis(long int ms);

  ///This function returns a timespec with the specified microseconds
  static struct timespec Micros(long int us);

  ///Converts a timespce to unsigned long usecs
  static unsigned long convert_us(struct timespec t1);

};

#endif
