#include "JobTrace.h"

#include "TimeUtil.h"

#include <sstream>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

///This function sets the attributes of the object
void JobTrace::setTrace(struct timespec ts, _thread_type tt, unsigned int ti, _task_action ta) {
  timestamp = ts;
  thread_type = tt;
  thread_id = ti;
  task_action = ta;
}

///This function converts the trace to string for file output
string JobTrace::toString() {
  stringstream out;

  out << TimeUtil::convert_us(timestamp) <<  "," << thread_id << "," << task_action;

  return out.str();
}
