#ifndef _RUNNABLE_H
#define _RUNNABLE_H

#include "pthread/Thread.h"
#include "Deadline.h"
#include "util/Enumerations.h"

class Simulation;
class Scheduler;
class Task;
class Criteria;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Runnable : public Thread {
  /*********** VARIABLES ***********/
 protected:
  ///Pointer to simulation
  Simulation *sim;
  
  ///Runnable's schedulable criteria (this is the criteria used to schedule it)
  Criteria *criteria;
  
  ///Pointer to Task currently being 'executed' by the runnable
  Task *current_task;
  
  ///Pointer to the scheduler that this runnable belongs to
  Scheduler *parent;

  ///Auxiliary variable to hold the state of runnable
  _runnable_state state;

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

  ///This function joins the calling thread with the object's pthread
  void join();

  /*********** GETTERS AND SETTERS ***********/
  ///This function return the runnable's schedulable criteria
  Criteria* getCriteria();

  ///This function sets the runnable's parent
  void setParent(Runnable *p);
};
#endif
