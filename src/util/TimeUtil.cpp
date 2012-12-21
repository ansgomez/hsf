#include "TimeUtil.h"

#include "Enumerations.h"
#include "Operators.h"

#include <time.h>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

struct timespec TimeUtil::offset;

/********************* MEMBER FUNCTIONS *********************/

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

///Sets the relative-time offset to current time
void TimeUtil::setOffset()
{
  clock_gettime(CLOCK_REALTIME, &offset);
}

///This function returns a timespec with the specified seconds
struct timespec TimeUtil::Seconds(__time_t s) {
  struct timespec aux;

  aux.tv_sec = s;
  aux.tv_nsec = 0;

  return aux;
}

///This function returns a timespec with the specified milliseconds
struct timespec TimeUtil::Millis(long int ms) {
  struct timespec aux;


  if(ms < 1000) {
    aux.tv_sec = 0;
    aux.tv_nsec = ms*1000000;
  }
  else {
    aux.tv_sec = (int) (ms/1000);
    aux.tv_nsec = (ms-aux.tv_sec*1000)*1000000;
  }

  return aux;
}

///This function returns a timespec with the specified microseconds
struct timespec TimeUtil::Micros(long int us) {
  struct timespec aux;

  if(us < 1000000) {
    aux.tv_sec = 0;
    aux.tv_nsec = us*1000;
  }
  else {
    aux.tv_sec = (int) (us/1000000);
    aux.tv_nsec = (us-aux.tv_sec*1000000)*1000;
  }

  return aux;
}

unsigned long int TimeUtil::convert_us(struct timespec t1)
{
  unsigned long int ul = (unsigned long int) (t1.tv_sec)*1000000 + (unsigned long int)(t1.tv_nsec)/1000;
  return ul;
}
