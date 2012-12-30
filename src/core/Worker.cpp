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

  criteria = new InclusiveCriteria();
  //arrival_times.reserve(100);

  relativeDeadline = TimeUtil::Millis(20);

  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&arrival_sem, 0, 1); //mutex semaphore
  sem_init(&wrapper_sem, 0, 0); //signal semaphore
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

  sem_wait(&arrival_sem);
    //Erase old arrival time from vector
    arrival_times.pop_front();

    //If there are any jobs left on queue, register new head
    if(arrival_times.size() > 0) {
      //Update objeect's schedulable criteria
      if(criteria != NULL) {
	criteria->setArrivalTime(arrival_times.front());
	criteria->setDeadline(arrival_times.front()+relativeDeadline);
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
	parent->job_finished(id);
      }
      else {
	cout << "Worker::job_finished - parent is null!\n";
      }
    }
  sem_post(&arrival_sem);
}

///This function will be called by the dispatcher thread, and will post to the wrapper_sem
void Worker::new_job() {

  //add arrival time before critical section
  arrival_times.push_back(TimeUtil::getTime());

  sem_wait(&arrival_sem);

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
    //If there is an active job, job_finished() will take care of 'registering' this new job with parent    
  //End critical section
  sem_post(&arrival_sem);

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
