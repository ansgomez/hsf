#ifndef _RUNNABLE_H
#define _RUNNABLE_H

#include "Thread.h"
#include "Deadline.h"
#include "Enumerations.h"

class Simulation;
class Scheduler;
class Task;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Runnable : public Thread {
  /*********** VARIABLES ***********/
 protected:
  ///Pointer to simulation
  Simulation *sim;
  
  ///Runnable's current deadline. Might be used by a higher level scheduler's algorithm
  Deadline deadline;
  
  ///Pointer to Task currently being 'executed' by the runnable
  Task* current_task;
  
  ///Pointer to the scheduler that this runnable belongs to
  Scheduler* parent;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs simulation pointer and an id
  Runnable(Simulation *s, unsigned int _id);

  /*********** INHERITED FUNCTIONS ***********/
  ///This function will be defined by subclasses
  virtual void wrapper();

  ///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
  virtual void activate();

  ///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
  virtual void deactivate();
};
#endif
