#include "Worker.h"
//#include "defines.h"

#include "Scheduler.h"
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

/********************* CONSTRUCTOR *********************/

Worker::Worker(Simulation *s, Scheduler *sched, unsigned int _id, _task_load tl) : Runnable(s, _id) {
#if _INFO == 1
  cout << "Creating Worker with ID: " << _id << endl;
#endif

  sim = s;
  id = _id;
  scheduler = sched;
  type = worker;

  worker_activated = -1;

  //Register worker id with simulation
  sim->add_worker_id(_id);

  sem_init(&wrapper_sem, 0, 0);
}

/********************* INHERITED FUNCTIONS *********************/

void Worker::wrapper() {
  while (sim->isSimulating() == 1) {
    sem_wait(&wrapper_sem);

    sim->getTraces()->add_trace(worker, id, task_start);

    if( sim->isSimulating() == 1) {
      if(load != NULL) {
	if(id==47)
	  cout << "+Worker 47 is firing @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;

	load->fire();    

	if(id==47)
	  cout << "-Worker 47 finished @t=" << TimeUtil::convert_us(TimeUtil::getTime(),relative) << endl;

      }
      else {
	cout << "Worker error: load is null!\n";
      }
    }

    sim->getTraces()->add_trace(worker, id, task_end);
  }

#if _INFO == 1 
  cout << "Worker " << id << " exiting Worker::wrapper\n";
#endif
}

///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
void Worker::activate() {
  if(id==47)
    cout << "*Worker 47 is activated @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;

  //Trace 'active' only if it wasn't active before
  if(worker_activated != 1) {
    sim->getTraces()->add_trace(worker, id, sched_start);
    worker_activated = 1;
  }

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_active_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}

///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
void Worker::deactivate() {
  if(id==47)
    cout << "*Worker 47 is deactivated @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;

  if(worker_activated != 0) {
    sim->getTraces()->add_trace(worker, id, sched_end);
    worker_activated = 0;
  }

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}

///Thisfunction joins the calling thread with the object's pthread
void Worker::join() {
#if _INFO == 1
  cout << "W: Attempting to joining thread " << id << endl;
#endif

  /*
  if(thread==NULL) 
    cout << "Worker::join - NULL PROBLEM\n";
  */

  pthread_join(thread, NULL);

#if _INFO == 1
  cout << "W: Successfully joined thread " << id << endl;
#endif
}

/********************* MEMBER FUNCTIONS *********************/

void Worker::setLoad(Task *t) {
  load = t;
}

void Worker::new_job() {
#if _INFO==1
  cout << "+Worker " << id << " has sem posted\n";
#endif
  //TODO register the new job even to the parent scheduler
  //parent->new_job(this);
  sem_post(&wrapper_sem);
}
