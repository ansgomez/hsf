#include "Dispatcher.h"

#include "Trace.h"
#include "Worker.h"
#include "Thread.h"
#include "Priorities.h"
#include "Enumerations.h"

#include <time.h>
#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/********************* CONSTRUCTOR *********************/

///Constructor needs Simulation pointer, and a disp_id
Dispatcher::Dispatcher (Simulation *s, unsigned int _id) : Thread(s,_id)
{
#if _INFO == 1
  cout << "++New Dispatcher - " << _id << "\n";
#endif

  sim = s;
  id = _id;
  type = dispatcher;

  //By default periodic
  periodicity = periodic;

  //Offset is initially 0
  offset.tv_sec = 0;
  offset.tv_nsec = 0;

  worker = NULL;
}

/********************* SETTER FUNCTIONS *********************/
///This function sets the worker
void Dispatcher::setWorker(Worker *w) {
  worker = w;
}

///This function sets the dispatcher's periodicity
void Dispatcher::setPeriodicity(_task_periodicity t) {
  periodicity = t;
}

///This function sets the dispatcher's offset
void Dispatcher::setOffset(struct timespec o) {
  offset = o;
}

/********************* INHERITED FUNCTIONS *********************/
///This is the pthread's wrapper function
void Dispatcher::wrapper() {
  start();
}

/********************* MEMBER FUNCTIONS *********************/

///This function waits for offset, and then calls dispatch -> which has the flagged loop.
void Dispatcher::start() {
  struct timespec rem;

  //if offset != 0, sleep before dispatching
  if(offset.tv_nsec != 0 || offset.tv_sec !=0) {
    nanosleep(&offset, &rem);
  }

  dispatch();
}

///This virtual function should be implemented by the subclasses
void Dispatcher::dispatch() {
  //empty
  cout << "Dispatcher::dispatch - This should not print!\n";
}

///This function sets the dispatcher's priority to DISP_PR
void Dispatcher::activate() {

#if _DEBUG == 1
  cout << "Dispatcher " << id << " now has DISP_PR\n";
#endif

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_disp_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}
