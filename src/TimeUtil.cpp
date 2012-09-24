#include "TimeUtil.h"

#include "Enumerations.h"
#include "Operators.h"

#include <time.h>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

struct timespec TimeUtil::start_time;
struct timespec TimeUtil::offset;

///This function returns a timespec with the current time
struct timespec TimeUtil::getTime()
{
  struct timespec aux;
  clock_gettime(CLOCK_REALTIME, &aux);
  return aux;
}

///This function returns a timespec with the current time
struct timespec TimeUtil::getTime(enum _time_type type)
{
  struct timespec aux;
  clock_gettime(CLOCK_REALTIME, &aux);
  
  if(type == relative) {
    aux = aux - offset;
  }

  return aux;
}

///Converts a timespce to unsigned long usecs
unsigned long int TimeUtil::convert_us(struct timespec t1)
{
  unsigned long int ul = (unsigned long int) (t1.tv_sec)*1000000 + (unsigned long int)(t1.tv_nsec)/1000;
  return ul;
}

///Converts a timespec to unsigned long usecs
unsigned long TimeUtil::convert_us(struct timespec t1, enum _time_type type)
{
  if(type == relative) {
    t1 = t1-offset;
  }

  unsigned long ul = (unsigned long) (t1.tv_sec)*1000000 + (unsigned long)(t1.tv_nsec)/1000;
  return ul;
}

///Sets the relative-time offset to current time
void TimeUtil::setOffset()
{
  clock_gettime(CLOCK_REALTIME, &offset);
}

