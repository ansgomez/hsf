#include "BusyWait.h"

#include "TimeUtil.h"
#include "Operators.h"
#include "Dispatcher.h"
#include "Simulation.h"
#include "Worker.h"

#include <iostream>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/********************* CONSTRUCTOR *********************/

///The constructor requires a pointer to the simulation, its own dispatcher, and the WCET
BusyWait::BusyWait(Simulation *s, Dispatcher *disp, struct timespec wcet) : Task(s, disp, busy_wait) {
  WCET = wcet;
}

/********************* MEMEBER FUNCTIONS *********************/

void BusyWait::fire() {
#if _DEBUG == 1
  cout << "+Worker " << dispatcher->worker->getId() << " began execution @t="<< TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
#endif

  unsigned int start, end, count=0, wcet_us = TimeUtil::convert_us(WCET);
  
  start = TimeUtil::convert_us(TimeUtil::getTime());

  //BUSY WAIT LOOP
  do {
    end = TimeUtil::convert_us(TimeUtil::getTime());

    if( (end - start) > 100) {
      count += 100;
      start = end;

      //if(count % 9000 == 0)
      //cout << "Count: " << count << " - WCET " << wcet_us << endl;
    }
  } while ( count < wcet_us && sim->isSimulating()==1 ); 

#if _DEBUG == 1
  cout << "-Worker " << dispatcher->worker->getId() << " finished execution@t="<< TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
#endif

}
