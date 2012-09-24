#include "Scheduler.h"
#include "Simulation.h"


#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Scheduler::Scheduler(Simulation *s, unsigned int id) : Runnable(s, &sched_id){
  sim = s;
  sched_id = id;
  thread_type = scheduler;
}

///This function adds a load to the scheduler (could be another scheduler, or a worker)
void Scheduler::add_load(Runnable *new_load) {
  load.push_back(new_load);
}

///This is the pthread's wrapper function
void Scheduler::wrapper() {
  schedule();

  pthread_exit(NULL);
  return;
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
