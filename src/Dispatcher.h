#ifndef _DISPATCHER_H
#define _DISPATCHER_H

#include "Thread.h"
#include "Enumerations.h"

#include <time.h>

using namespace std;

class Simulation;
class Worker;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Dispatcher : public Thread {
  /*********** VARIABLES ***********/
 protected:
  ///Pointer to simulation
  Simulation *sim;
  
  ///Periodicity of the task to be dispatched
  _task_periodicity periodicity;
  
  ///This is the (constant) relative deadline of the associated task
  struct timepsec relativeDeadline;

  ///When this is set, the dispatcher will sleep for the specified time before beginning to dispatch
  struct timespec offset;
    
 public:
  ///Pointer to the worker associated to the dispatcher
  Worker *worker;

  /*********** CONSTRUCTOR ***********/
  ///Contructor needs Simulation pointer, and a disp_id
  Dispatcher (Simulation *s, unsigned int id);

  /*********** INHERITED FUNCTIONS ***********/
  ///This is the pthread's wrapper function, calls dispatch -> which has the flagged loop.
  void wrapper();

  /*********** MEMBER FUNCTIONS ***********/
  ///This function was a flagged loop that activates the Worker according to the task periodicity
  virtual void dispatch();

  ///This function assignes DISP_PR to the thread
  void activate();  

  /*********** GETTER AND SETTER FUNCTIONS ***********/
  ///This function gets the relative deadline
  struct timespec getRelativeDeadline();
  ///This function sets the relative deadline
  void setRelativeDeadline(struct timespec aux);
  ///This function sets the associated worker
  void setWorker(Worker *w);
  ///This function sets the tasks's periodicity
  void setPeriodicity(_task_periodicity t);
  ///This function sets the dispatcher's offset
  void setOffset(struct timespec o);
};
#endif
