#include "core/Worker.h"

#include "core/Criteria.h"
#include "core/ResourceAllocator.h"
#include "core/Simulation.h"
#include "core/Task.h"
#include "criteria/InclusiveCriteria.h"
#include "pthread/Priorities.h"
#include "results/Statistics.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

#define _INFO 0
#define _DEBUG 0

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

Worker::Worker(ResourceAllocator *p, unsigned int _id, _task_load tl) : Runnable(_id) {
  #if _INFO == 1
  cout << "++New Worker \t - " << _id << endl;
  #endif

  parent = p;
  thread_type = worker;
  task_load = tl;
  Statistics::addWorkerId(_id);

  criteria = new InclusiveCriteria();
  //arrival_times.reserve(100);

  //Default value for relativeDeadline
  relativeDeadline = TimeUtil::Millis(20);

  //Semaphore initialization
  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&arrival_sem, 0, 1); //mutex semaphore
  sem_init(&wrapper_sem, 0, 0); //signal semaphore
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///This join function takes into account the worker's unblocking mechanism
void Worker::join() {
  if(parent!=NULL) {
    parent->join();
  }

  //Post to sem in case worker is blocked
  sem_post(&wrapper_sem);
  sem_post(&activation_sem);
  sem_post(&arrival_sem);

  join2();
}

///This inherited function will be executed by the worker thread
void Worker::wrapper() {

  //Wait until the simulation is initialized
  while( !Simulation::isInitialized() );

  #if _INFO == 1
  cout << "Worker: " << id << " begining execution\n";
  #endif

  while( Simulation::isSimulating() ) {

    //Wait for new job
    sem_wait(&wrapper_sem);

    Statistics::addTrace(worker, id, task_start);

    if( Simulation::isSimulating() ) {
      if(task != NULL) {
	task->fire();    
      }
      else {
	cout << "Worker error: task is null!\n";
      }
    }

    //TODO if(now() > deadline()) -> add to missed_deadlines

    Statistics::addTrace(worker, id, task_end);

    //Handle the end of the current job (might regise job_finished or renew_job with parent)
    job_finished();
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

  if( !Simulation::isSimulating() ) {
    return;
  }

  //If there are any jobs left on queue, register new head
  if(arrival_times.size() > 1) {
    //Update object's schedulable criteria
    if(criteria != NULL) {
      criteria->setArrivalTime(arrival_times[1]);
      criteria->setDeadline(arrival_times[1]+relativeDeadline);
    }
    else {
      cout << "Worker::job_finished - criteria is null!\n";
    }
    
    //Notify parent of new arrival
    if (parent != NULL ) {
      parent->renew_job(this);
    }
    else {
      cout << "Worker::job_finished - parent is null!\n";
    }
  }
  //If no jobs are pending, remove from parent
  else {
    //Remove job from parents' queue
    if(parent != NULL) {
      //Clear schedulable criteria
      criteria->setArrivalTime(TimeUtil::Millis(0));
      criteria->setDeadline(TimeUtil::Millis(0));

      sem_wait(&arrival_sem);
        arrival_times.pop_front(); //Erase old arrival time
      sem_post(&arrival_sem);

      parent->job_finished(id); //Register event with the parent
    }
    else {
      cout << "Worker::job_finished - parent is null!\n";
    }
  }
}

///This function will be called by the dispatcher thread, and will post to the wrapper_sem
void Worker::new_job() {
  //add arrival time before critical section
  struct timespec aux = TimeUtil::getTime();

  #if _DEBUG==1
  cout << "Worker::newjob() is waiting\n";
  #endif

  sem_wait(&arrival_sem);
    arrival_times.push_back(aux);
  sem_post(&arrival_sem);

  #if _DEBUG==1
  cout << "Worker::newjob() is processing\n";
  #endif

  //If there were no active jobs before, register event
  if(arrival_times.size() == 1) {

    //Update schedulable criteria
    if( criteria != NULL ) {
      criteria->setArrivalTime(arrival_times.front());
      criteria->setDeadline(arrival_times.front() + relativeDeadline);
    }
    else {
      cout << "Worker::new_job - criteria is null!\n";
    }
      
    //Notify parent of new arrival
    if (parent != NULL ) {
      parent->new_job(this);
    }
    else {
      cout << "Worker::new_job - parent is null!\n";
    }
  }

  //If there is an active job, job_finished() will take care of 
  //'registering' this new job with parent    

  //Signal the worker thread
  sem_post(&wrapper_sem);
}

/*********** GETTER AND SETTER FUNCTIONS ***********/
///This function gets the relative deadline
struct timespec Worker::getRelativeDeadline() {
  return relativeDeadline;
}

///This function sets the worker's load
void Worker::setTask(Task *t) {
  task = t;
}

///This function sets the relative deadline
void Worker::setRelativeDeadline(struct timespec aux) {
  relativeDeadline = aux;
}
