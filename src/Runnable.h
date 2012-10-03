#ifndef _RUNNABLE_H
#define _RUNNABLE_H

//#include "Scheduler.h"
//#include "Task.h"
#include "Thread.h"
#include "Deadline.h"
#include "Enumerations.h"

class Simulation;
class Scheduler;
class Task;

class Runnable : public Thread {
 protected:
  Simulation *sim;
  
  Deadline deadline;
  
  Task* current_task;
  
  Scheduler* parent;

 public:
  ///Constructor needs simulation pointer and an id
  Runnable(Simulation *s, unsigned int _id);

  virtual void wrapper();

  ///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
  virtual void activate();

  ///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
  virtual void deactivate();
};
#endif
