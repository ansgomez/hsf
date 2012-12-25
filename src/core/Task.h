#ifndef _TASK_H
#define _TASK_H

#include "util/Enumerations.h"

#include <time.h>

class Simulation;
class Dispatcher;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/
 
class Task {
 
 protected:
 
  /*********** VARIABLES ***********/
  
  ///Pointer to the simulation
  Simulation *sim;

  ///Pointer to the task's dispatcher
  Dispatcher *dispatcher;

  ///Varible describing the task's type of load (video, busy_wait)
  _task_load task_load;

 public:
 
  /*********** CONSTRUCTOR ***********/
  
  ///Constructor needs pointer to simulation, dispatcher, and its load type
  Task(Simulation *s, Dispatcher *disp, _task_load load);

  /*********** MEMBER FUNCTIONS ***********/

  ///This function performs one task (whether it's decoding one frame, or one busy-wait loop)
  virtual void fire();

};
#endif
