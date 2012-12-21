#ifndef _WORKER_H
#define _WORKER_H

//#include "Scheduler.h"
#include "Runnable.h"
#include "Enumerations.h"

#include <semaphore.h>

class Scheduler;
class Simulation;
class Task;

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Worker : public Runnable {
  /*********** VARIABLES ***********/
 private:
  ///Pointer to simulation
  Simulation *s;

  ///Pointer to the task to be executed by the worker
  Task *load;
  
  ///Type of task load (busy_wait, video)
  enum _task_load task_load;

  ///Vector to hold arrival times of active tasks
  vector<struct timespec> arrival_times;
  
  ///Pointer to the scheduler handling this worker
  Intermediary * parent;

  ///Semaphore to control the call to the fire() function
  sem_t wrapper_sem;

  ///Semaphore to control the activation/deactivation of the thread
  sem_t activation_sem;

  ///Semaphore to control access to the arrivalTime vector
  sem_t arrival_sem;

  /*********** CONSTRUCTOR ***********/
 public:
  Worker(Simulation *s, Scheduler *sched, unsigned int id, _task_load tl);

  /*********** INHERITED FUNCTIONS ***********/
  ///This inherited function will be executed by the worker thread
  void wrapper();
  
  ///This function give the worker thread the ACTIVE_PR priority
  void activate();

  ///This function give the worker thread the INACTIVE_PR priority
  void deactivate();

  ///Thisfunction joins the calling thread with the object's pthread
  void join();

  /*********** MEMBER FUNCTIONS ***********/
  ///This function will be called by the dispatcher thread, and will post to the wrapper_sem
  void new_job(struct timespec realtiveDeadline);

  /*********** GETTER AND SETTER FUNCTIONS ***********/
  ///This function gets the relative deadline
  struct timespec getRelativeDeadline();
  ///This function sets the relative deadline
  void setRelativeDeadline(struct timespec aux);

  ///This function returns the task's criteria
  Criteria getCriteria();

  ///This function sets the worker's task
  void setLoad(Task *t);
};
#endif
