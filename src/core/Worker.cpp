#include "core/Worker.h"

#include "core/Criteria.h"
#include "core/Intermediary.h"
#include "core/Simulation.h"
#include "core/Task.h"
#include "pthread/Priorities.h"
#include "results/Statistics.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

#define _INFO 1

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

Worker::Worker(Intermediary *p, unsigned int _id, _task_load tl) : Runnable(_id) {
#if _INFO == 1
  cout << "++New Worker \t - " << _id << endl;
#endif

  parent = p;
  thread_type = worker;

  //Register worker id with simulation
  //sim->add_worker_id(_id);

  arrival_times.reserve(100);

  relativeDeadline = TimeUtil::Millis(20);

  sem_init(&wrapper_sem, 0, 0); //signal semaphore
  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&arrival_sem, 0, 1); //mutex semaphore
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///This function joins the calling thread with the object's pthread
void Worker::join() {
  //Post to sem in case worker is blocked
  sem_post(&wrapper_sem);

  pthread_join(thread, NULL);
}

///This inherited function will be executed by the worker thread
void Worker::wrapper() {
#if _INFO == 1
  cout << "Worker: " << id << " waiting for initialization\n";
#endif
  //Wait until the simulation is initialized
  while( !Simulation::isInitialized());

#if _INFO == 1
  cout << "Worker: " << id << " begining execution\n";
#endif

  while (Simulation::isSimulating()) {
    sem_wait(&wrapper_sem);

    Statistics::addTrace(worker, id, task_start);

    if( Simulation::isSimulating() ) {
      if(load != NULL) {
	load->fire();    
      }
      else {
	cout << "Worker error: load is null!\n";
      }
    }

    //TODO if(now() > deadline()) -> add to missed_deadlines

    Statistics::addTrace(worker, id, task_end);

    //remove arrival_time from schedulable criteria
    parent->job_finished(id);
  }
}

/**** FROM RUNNABLE ****/

///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
void Worker::activate() {
  if(state == activated) {
    cout << "Worker::activate error - already active!\n";
  }

  sem_wait(&activation_sem);

  Statistics::addTrace(worker, id, sched_start);

  setPriority(Priorities::get_active_pr());
  
  state = activated;

  sem_post(&activation_sem);
}

///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
void Worker::deactivate() {
  if(state == deactivated ) {
    cout << "Worker::deactivate error - already deactivated!\n";
  }

  sem_wait(&activation_sem);
  
  Statistics::addTrace(worker, id, sched_end);

  setPriority(Priorities::get_inactive_pr());    
  
  state = deactivated;

  sem_post(&activation_sem);
}

/*********** MEMBER FUNCTIONS ***********/

///This function erases the head of the active_queue, and updates any pending events
void Worker::job_finished() {

  if(Simulation::isSimulating() != 1) {
    return;
  }

  sem_wait(&arrival_sem);

  //Erase old arrival time from vector
  arrival_times.erase(arrival_times.begin());

  //If there are any jobs left on queue, register new head
  if(arrival_times.size() > 0) {
    //Update objeect's schedulable criteria
    criteria->setDeadline(arrival_times[0]+relativeDeadline);

    //Notify parent of new head
    parent->renew_job(this);
  }
  //If no jobs are pending, remove from parent
  else {
    //Remove job from parents' queue
    parent->job_finished(id);
  }

  sem_post(&arrival_sem);
}

///This function will be called by the dispatcher thread, and will post to the wrapper_sem
void Worker::new_job() {

  cout << "Worker " << id << " got a new job!";

  sem_wait(&arrival_sem);
  arrival_times.push_back(TimeUtil::getTime() + relativeDeadline);

  //If there were no active jobs before, register event
  if(arrival_times.size() == 1) {
    //Update schedulable criteria
    criteria->setDeadline(arrival_times[0]);

    //Notify parent of new arrival
    parent->new_job(this);
  }
  //Otherwise, the worker is already executing a job and when that finished, the next job will be renewed with the parent

  sem_post(&arrival_sem);

  //Post to worker thread
  sem_post(&wrapper_sem);

  cout << "Exiting new job\n";
}

/********************* GETTER AND SETTER FUNCTIONS *********************/
///This function gets the relative deadline
struct timespec Worker::getRelativeDeadline() {
  return relativeDeadline;
}

///This function sets the worker's load
void Worker::setLoad(Task *t) {
  load = t;
}

///This function sets the relative deadline
void Worker::setRelativeDeadline(struct timespec aux) {
  relativeDeadline = aux;
}
