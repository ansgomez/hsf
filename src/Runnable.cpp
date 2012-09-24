#include "Runnable.h"

#include "Task.h"
#include "Scheduler.h"
#include "Simulation.h"
#include "Priorities.h"
#include "Trace.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Runnable::Runnable(Simulation *s, unsigned int *_id) : Thread() {
  sim = s;
  id = _id;
}

///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
void Runnable::activate() {

  sim->getTraces()->add_trace(scheduler, *id, sched_start);

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_active_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}

///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
void Runnable::deactivate() {
  sim->getTraces()->add_trace(scheduler, *id, sched_end);

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}

void Runnable::wrapper() {
  //empty
  cout << "Runnable::wrapper - This should not print!\n";
}

