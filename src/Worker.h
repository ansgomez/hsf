#ifndef _WORKER_H
#define _WORKER_H

//#include "Scheduler.h"
#include "Runnable.h"
#include "Enumerations.h"

#include <semaphore.h>

class Scheduler;
class Simulation;
class Task;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Worker : public Runnable {
 private:
  Simulation *s;

  Task *load;
  
  enum _task_load task_load;
  
  Scheduler * scheduler;

  sem_t wrapper_sem;

 public:
  Worker(Simulation *s, Scheduler *sched, unsigned int id, _task_load tl);
  void new_job();

  void wrapper();

  void setLoad(Task *t);

  /**** INHERITED METHODS ****/
  void activate();
  void deactivate();
};
#endif
