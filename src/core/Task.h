#ifndef _TASK_H
#define _TASK_H

#include "util/Enumerations.h"

#include <time.h>

class Dispatcher;
class Simulation;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/
 
class Task {
 
 protected:
 
  /*********** VARIABLES ***********/
  
  ///Varible describing the task's type of load (video, busy_wait)
  _task_load task_load;

  ///Pointer to the task's dispatcher
  Dispatcher *dispatcher;

  ///Pointer to the simulation
  Simulation *sim;

 public:
 
  /*********** CONSTRUCTOR ***********/
  
  ///Constructor needs pointer to simulation, dispatcher, and its load type
  Task(Simulation *s, Dispatcher *disp, _task_load load);

  /*********** MEMBER FUNCTIONS ***********/

  ///This function performs one task (whether it's decoding one frame, or one busy-wait loop)
  virtual void fire();

};
#endif
