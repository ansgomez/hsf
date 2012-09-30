#ifndef _TASK_H
#define _TASK_H

#include "Deadline.h"
#include "Enumerations.h"

#include <time.h>

class Simulation;
class Dispatcher;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Task {
 protected:
  Deadline deadline;

  Simulation *sim;

  Dispatcher * dispatcher;

  struct timespec arrival_time;

  unsigned int priority;

  _task_load task_load;

 public:
  Task(Simulation *s, Dispatcher *disp, _task_load load);

  virtual void fire();

};
#endif
