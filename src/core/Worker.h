#ifndef _WORKER_H
#define _WORKER_H

#include "core/Runnable.h"
#include "util/Enumerations.h"

#include <semaphore.h>
#include <vector>

class ResourceAllocator;
class Task;

using namespace std;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Worker : public Runnable {

 private:

  /*********** VARIABLES ***********/

  ///Vector to hold arrival times of active tasks
  vector<struct timespec> arrival_times;
  
  ///Pointer to the scheduler handling this worker
  ResourceAllocator* parent;

  ///Pointer to the task to be executed by the worker
  Task* load;
  
  ///Type of task load (busy_wait, video)
  enum _task_load task_load;

  ///This variable determines each job's deadline (deadline = arrival_time + relativeDeadline)
  struct timespec relativeDeadline;

  /**** SEMAPHORES ****/

  ///Semaphore to control the activation/deactivation of the thread
  sem_t activation_sem;
  ///Semaphore to control access to the arrivalTime vector
  sem_t arrival_sem;
  ///Semaphore to control the call to the fire() function
  sem_t wrapper_sem;

  /*********** CONSTRUCTOR ***********/
 public:
  Worker(ResourceAllocator *p, unsigned int _id, _task_load tl);

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM THREAD ****/
  
  ///Thisfunction joins the calling thread with the object's pthread
  void join();

  ///This inherited function will be executed by the worker thread
  void wrapper();
  
  /**** FROM RUNNABLE ****/

  ///This function give the worker thread the ACTIVE_PR priority
  void activate();

  ///This function give the worker thread the INACTIVE_PR priority
  void deactivate();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function erases the head of the active_queue, and updates any pending events
  void job_finished();

  ///This function will be called by the dispatcher thread, and will post to the wrapper_sem
  void new_job();

  /*********** GETTER AND SETTER FUNCTIONS ***********/
  ///This function gets the relative deadline
  struct timespec getRelativeDeadline();
  ///This function sets the worker's task
  void setLoad(Task *t);
  ///This function sets the relative deadline
  void setRelativeDeadline(struct timespec aux);
};
#endif
