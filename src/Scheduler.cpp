#include "Scheduler.h"
#include "Simulation.h"
#include "Priorities.h"
#include "Trace.h"

#include <pthread.h>
#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Scheduler::Scheduler(Simulation *s, unsigned int _id) : Runnable(s, id){
  sim = s;
  id = _id;
  thread_type = scheduler;
}

///This function adds a load to the scheduler (could be another scheduler, or a worker)
void Scheduler::add_load(Runnable *new_load) {
  load.push_back(new_load);
}

///This is the pthread's wrapper function
void Scheduler::wrapper() {
  schedule();
}

///This function rewrites the deactivate method both the scheduler as well as its load
void Scheduler::deactivate() {
  //first deactivate it's active load
  load[active_index]->deactivate();

  //now the old deactivate() for itself
  sim->getTraces()->add_trace(scheduler, id, sched_end);

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}

void Scheduler::schedule() {
  //empty
  cout << "Scheduler::schedule - This should not print!\n";
}

void Scheduler::new_job(Worker *w) {
  //empty
  cout << "Scheduler::new_job - This should not print!\n";
}

void Scheduler::job_finished(int worker) {
  //empty
  cout << "Scheduler::job_finished - This should not print!\n";
}


///This function waits for the scheduler's load to join
void Scheduler::join_all() {
  for(unsigned int c=0; c<load.size(); c++) {
    pthread_join(*(load[c]->getPthread()), NULL);
  }
}
