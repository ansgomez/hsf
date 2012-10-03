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

Scheduler::Scheduler(Simulation *s, unsigned int _id, int _level) : Runnable(s, _id){
  sim = s;
  id = _id;
  type = scheduler;
  active_index = -1;
  level = _level;
}

///This function adds a load to the scheduler (could be another scheduler, or a worker)
void Scheduler::add_load(Runnable *new_load) {
  load.push_back(new_load);
}

///This is the pthread's wrapper function
void Scheduler::wrapper() {
  schedule();
  cout << "Scheduler " << id << " has exited Scheduler()\n";
}

///This function rewrites the activate method to activate both the scheduler as well as its load
void Scheduler::activate() {
  cout << "Scheduler::activate() - This should not print!\n";
  /*
#if _INFO == 1
  cout << "Scheduler " << id << " is now active\n";
#endif 

  if(parent != NULL) {
    //TODO add trace for non-top_sched deactivations
  }

  sim->getTraces()->add_trace(scheduler, id, sched_start);

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_server_pr(0); //server priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);

  //if there was an active load, reactivate it
  if(active_index != -1) {
    load[active_index]->activate();
  }
  */
}


///This function rewrites the deactivate method to deactivate both the scheduler as well as its load
void Scheduler::deactivate() {
  cout << "Scheduler::deactivate() - This should not print!\n";
  /*
  //first deactivate it's active load
  if(active_index != -1) {
    load[active_index]->deactivate();
  }
 
  //TODO if TDMA, must post to interrupt_sem

  //now the old deactivate() for itself
  sim->getTraces()->add_trace(scheduler, id, sched_end);

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
  */
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
void Scheduler::join() {
  for(unsigned int c=0; c<load.size(); c++) {

#if _DEBUG==0
    cout << "Sched: " << id << " waiting for load: " << c << endl;
#endif

    if( load[c] != NULL) {
      load[c]->join();
    }

#if _DEBUG==0
    cout << "Sched: " << id << " has joined load: " << c << endl;
#endif

  }

#if _DEBUG==0
    cout << "Sched: " << id << " exiting join_all!\n";
#endif
}
