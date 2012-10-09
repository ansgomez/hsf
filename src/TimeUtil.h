#ifndef _TIMEUTIL_H
#define _TIMEUTIL_H

#include "Enumerations.h"

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

  ///Converts a timespce to unsigned long usecs
  static unsigned long convert_us(struct timespec t1);

  ///Converts a timespce to unsigned long usecs
  static unsigned long convert_us(struct timespec t1, enum _time_type t);

  ///Converts a timespce to unsigned long msecs
  static unsigned long convert_ms(struct timespec t1);

  //Sets the relative-time offset to current time
  static void setOffset();

};

#endif
