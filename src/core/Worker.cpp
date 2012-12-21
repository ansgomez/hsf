#include "Worker.h"

#include "Intermediary.h"
#include "Trace.h"
#include "Task.h"
#include "Simulation.h"
#include "Runnable.h"
#include "Priorities.h"
#include "TimeUtil.h"
#include "Enumerations.h" 

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/

Worker::Worker(Simulation *s, Intermediary *p, unsigned int _id, _task_load tl) : Runnable(s, _id) {
#if _INFO == 1
  cout << "Creating Worker with ID: " << _id << endl;
#endif

  sim = s;
  id = _id;
  parent = p;
  type = worker;

  //Register worker id with simulation
  sim->add_worker_id(_id);

  sem_init(&wrapper_sem, 0, 0); //signal semaphore
  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&arrival_sem, 0, 1); //mutex semaphore
}

/*********** INHERITED FUNCTIONS ***********/

void Worker::wrapper() {
  //Wait until the simulation is initialized
  while(sim->isInitialized() == 0);

  while (sim->isSimulating() == 1) {
    sem_wait(&wrapper_sem);

    sim->getTraces()->add_trace(worker, id, task_start);

    if( sim->isSimulating() == 1) {
      if(load != NULL) {
	load->fire();    
      }
      else {
	cout << "Worker error: load is null!\n";
      }
    }

    sim->getTraces()->add_trace(worker, id, task_end);

    //remove arrival_time from schedulable criteria
    job_finished();
  }
}

///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
void Worker::activate() {
  if(state == activated) {
    cout << "Worker::activate error - already active!\n";
  }

  sem_wait(&activation_sem);

  sim->getTraces()->add_trace(worker, id, sched_start);

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_active_pr();
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
  
  state = activated;

  sem_post(&activation_sem);
}

///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
void Worker::deactivate() {
  if(state == deactivated ) {
    cout << "Worker::deactivate error - already deactivated!\n";
  }

  sem_wait(&activation_sem);
  
  sim->getTraces()->add_trace(worker, id, sched_end);
    
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr();
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
  
  state = deactivated;

  sem_post(&activation_sem);
}

///This function joins the calling thread with the object's pthread
void Worker::join() {
  //Post to sem in case worker is blocked
  sem_post(&wrapper_sem);

  pthread_join(thread, NULL);
}

/*********** MEMBER FUNCTIONS ***********/

///This function will be called by the dispatcher thread, and will post to the wrapper_sem
void Worker::new_job(struct timespec realtiveDeadline) {

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
}

///This function erases the head of the active_queue, and updates any pending events
void Worker::job_finished() {

  if(sim->isSimulating != 1) {
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

/********************* GETTER AND SETTER FUNCTIONS *********************/
///This function gets the relative deadline
struct timespec getRelativeDeadline() {
  return relativeDeadline;
}

///This function sets the relative deadline
void setRelativeDeadline(struct timespec aux) {
  relativeDeadline = aux;
}

///This function sets the worker's load
void Worker::setLoad(Task *t) {
  load = t;
}
