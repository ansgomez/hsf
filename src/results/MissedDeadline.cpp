#include "results/MissedDeadline.h"

#include "util/TimeUtil.h"

#include <iostream>
#include <sstream>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/********************* MEMBER FUNCTIONS *********************/

///This function sets the attributes of the object
void MissedDeadline::setMissedDeadline(unsigned int ti, struct timespec at, struct timespec ft) {
  thread_id = ti;
  arrivalTime = at;
  finishTime = ft;  
}


///This function converts the trace to string for file output
string MissedDeadline::toString() {
  stringstream out;

  out << thread_id <<  "," << TimeUtil::convert_us(arrivalTime) << "," << TimeUtil::convert_us(finishTime) << endl;

  return out.str();
}

///This function returns the thread ID
unsigned int MissedDeadline::getId() {
  return thread_id;
}

///This function returns the job's arrival time
struct timespec MissedDeadline::getArrivalTime() {
  return arrivalTime;
}

///This function returns the jobs finish time
struct timespec MissedDeadline::getFinishTime() {
  return finishTime;
}
