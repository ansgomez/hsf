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

/********************* CONSTRUCTOR *********************/

///Constructor needs pointer to simulation as well as the scheduler's id and hierarchical level
Scheduler::Scheduler(Simulation *s, unsigned int _id, int _level) : Runnable(s, _id){
  sim = s;
  id = _id;
  thread_type = scheduler;
  level = _level;
}

/********************* INHERITED FUNCTIONS *********************/

///This is the pthread's wrapper function
void Scheduler::wrapper() {
  schedule();
}

///This function rewrites the activate method to activate both the scheduler as well as its load
void Scheduler::activate() {
  cout << "Scheduler::activate() - This should not print!\n";
}

///This function rewrites the deactivate method to deactivate both the scheduler as well as its load
void Scheduler::deactivate() {
  cout << "Scheduler::deactivate() - This should not print!\n";
}

///This function rewrites the join method to account for the scheduler's load
void Scheduler::join() {
  cout << "Scheduler::join() - This should not print!\n";
}

/********************* MEMBER FUNCTIONS *********************/

///This function performs the actual scheduling (figuring out the order of execution for its load)
void Scheduler::schedule() {
  //empty
  cout << "Scheduler::schedule - This should not print!\n";
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void Scheduler::new_job(Runnable *obj) {
  //empty
  cout << "Scheduler::new_job - This should not print!\n";
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void Scheduler::renew_job(int runnable_id) {
  //empty
  cout << "Scheduler::renew_job - This should not print!\n";
}

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void Scheduler::job_finished(int runnable_id) {
  //empty
  cout << "Scheduler::job_finished - This should not print!\n";
}
