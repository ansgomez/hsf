#ifndef _MISSEDDEADLINE_H
#define _MISSEDDEADLINE_H

#include "util/Enumerations.h"

#include <vector>
#include <string>

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class MissedDeadline {
/*********** VARIABLES ***********/
 private:
  ///This variable store the id of the thread the trace belongs to
  unsigned int thread_id;

  ///This variable will store the job's arrival time
  struct timespec arrivalTime;

  ///This variable will store the job's finish time
  struct timespec finishTime;

  /*********** MEMBER FUNCTIONS ***********/
 public:
  ///This function sets the attributes of the object
  void setMissedDeadline(unsigned int ti, struct timespec at, struct timespec ft);

  ///This function converts the trace to string for file output
  string toString();

  /*********** GETTER FUNCTIONS ***********/

  ///This function returns the thread ID
  unsigned int getId();

  ///This function returns the job's arrival time
  struct timespec getArrivalTime();

  ///This function returns the jobs finish time
  struct timespec getFinishTime();
};

#endif
