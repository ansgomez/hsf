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
  type = scheduler;
  active_index = -1;
}

///This function adds a load to the scheduler (could be another scheduler, or a worker)
void Scheduler::add_load(Runnable *new_load) {
  load.push_back(new_load);
}

///This is the pthread's wrapper function
void Scheduler::wrapper() {
  schedule();
}

///This function rewrites the activate method to activate both the scheduler as well as its load
void Scheduler::activate() {
#if _INFO == 1
  cout << "Scheduler " << id << " is now active\n";
#endif 

  if(parent != NULL) {
    //TODO add trace for non-top_sched deactivations
  }

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_server_pr(0); //server priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);

  //if there was an active load, reactivate it
  if(active_index != -1) {
    load[active_index]->deactivate();
  }
}


///This function rewrites the deactivate method to deactivate both the scheduler as well as its load
void Scheduler::deactivate() {
  //first deactivate it's active load
  if(active_index != -1) {
    load[active_index]->deactivate();
  }

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
