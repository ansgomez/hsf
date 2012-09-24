#include "BusyWait.h"

#include "TimeUtil.h"
#include "Operators.h"

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

BusyWait::BusyWait(Dispatcher *disp, struct timespec wcet) : Task(disp, busy_wait) {
  WCET = wcet;
}

void BusyWait::fire() {
  struct timespec start_time, end_time, us_100, count;

  //100 us
  us_100.tv_nsec = 100000;

    clock_gettime(CLOCK_REALTIME, &start_time);

  //BUSY WAIT LOOP
  do {
    clock_gettime(CLOCK_REALTIME, &end_time);

    //Increase counter (relaxation to accomodate VMs)
    if ( (end_time-start_time) > us_100) {
      count = count + us_100;
      start_time = end_time;
    }
  } while ( count < WCET);// && sim==1); 
}
