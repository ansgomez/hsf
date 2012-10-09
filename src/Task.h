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
  /*********** VARIABLES ***********/
  ///Pointer to the simulation
  Simulation *sim;

  ///The task's deadline, if any
  Deadline deadline;

  ///Pointer to the task's dispatcher
  Dispatcher * dispatcher;

  ///Variable holding the latests job's arrival time
  struct timespec arrival_time;

  ///Variable holding the task's assigned priority (if any)
  unsigned int priority;

  ///Varible describing the task's type of load (video, busy_wait)
  _task_load task_load;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs pointer to simulation, dispatcher, and its load type
  Task(Simulation *s, Dispatcher *disp, _task_load load);

  /*********** MEMBER FUNCTIONS ***********/
  virtual void fire();

};
#endif
