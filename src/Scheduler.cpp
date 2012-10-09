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
  type = scheduler;
  active_index = -1;
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

/********************* MEMBER FUNCTIONS *********************/

///This function adds a load to the scheduler (could be another scheduler, or a worker)
void Scheduler::add_load(Runnable *new_load) {
  load.push_back(new_load);
}

///This function performs the actual scheduling (figuring out the order of execution for its load)
void Scheduler::schedule() {
  //empty
  cout << "Scheduler::schedule - This should not print!\n";
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void Scheduler::new_job(Worker *w) {
  //empty
  cout << "Scheduler::new_job - This should not print!\n";
}

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void Scheduler::job_finished(int worker) {
  //empty
  cout << "Scheduler::job_finished - This should not print!\n";
}

///This function waits for the scheduler's load to join
void Scheduler::join() {
  for(unsigned int c=0; c<load.size(); c++) {

#if _DEBUG==1
    cout << "Sched: " << id << " waiting for load: " << c << endl;
#endif

    if( load[c] != NULL) {
      load[c]->join();
    }

#if _DEBUG==1
    cout << "Sched: " << id << " has joined load: " << c << endl;
#endif

  }

#if _DEBUG==1
    cout << "Sched: " << id << " exiting join_all!\n";
#endif
}
