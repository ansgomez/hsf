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

  protected:

  Simulation *sim;
  
  _task_periodicity periodicity;
  
  struct timespec offset;
    
 public:

  Worker *worker;

  /*********** CONSTRUCTOR ***********/
  ///Contructor needs Simulation pointer, and a disp_id
  Dispatcher (Simulation *s, unsigned int id);

  /*********** SETTER FUNCTIONS ***********/
  void setWorker(Worker *w);
  void setPeriodicity(_task_periodicity t);
  void setOffset(struct timespec o);

  /*********** INHERITED FUNCTIONS ***********/
  ///This is the pthread's wrapper function
  void wrapper();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function was a flagged loop that activates the Worker according to the task periodicity
  virtual void dispatch();

  ///This function waits for offset, and then calls dispatch -> which has the flagged loop.
  void start();

  //This function assignes DISP_PR to the thread
  void activate();  
};
#endif
