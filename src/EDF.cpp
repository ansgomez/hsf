#include "EDF.h"

#include "Scheduler.h"
#include "Runnable.h"
#include "Enumerations.h"
#include "Operators.h"
#include "Trace.h"
#include "Priorities.h"
#include "TimeUtil.h"

#include <time.h>
#include <iostream>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/
///Constructor needs pointer to simulation
EDF::EDf(Simulation *s, unsigned int id, int level) : Scheduler(s, id, level) {
#if _INFO==1
  cout << "Creating EDF with ID: " << id << endl;
#endif

  sem_init(&schedule_sem, 0, 1); //mutex semaphore
  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&newjob_sem, 0, 1); //mutex semaphore
  sem_init(&jobfinished_sem, 0, 1); //mutex semaphore
  sem_init(&preempt_sem, 0, 0); //sem used as signal
}

//TODO: DESTRUCTOR

/*********** INHERITED FUNCTIONS ***********/
///This function performs the actual scheduling (figuring out the order of execution for its load)
void EDF::schedule() {

  while(s->isSimulating() == 1) {
    //Wait for an event to ocurr
    sem_wait(&event_sem);
    //todo make sure that event_sem isn't posted more than once (unless necessary)

    sem_wait(&schedule_sem);

    //Deactivate currently active job (if any)
    if(active_queue[0] != NULL) {
      active_queue[0]->deactivate();
    }

    //Add any new jobs to the active queue
    sem_wait(&newjob_sem);
    while(newjob_queue.size() > 0) {
      //ordered insert
    }
    sem_post(&newjob_sem);

    //Remove any finished jobs from the active queue
    sem_wait(&jobfinished_sem);
    while(jobfinished_queue.size() > 0) {
      //ordered erase
    }
    sem_post(&jobfinished_sem);

    if(active_queue.size == 0) {
      continue;
    }

    active_queue[0]->activate();

    sem_post(&schedule_sem);
  }
}

///This function rewrites the activate method to 'activate' both the scheduler as well as its load - this runs in the higher level scheduler thread
void EDF::activate() {

  if(state == activated) {
    cout << "EDF::activate error - already activated!\n";
  }

  sem_wait(&activation_sem);
    
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_sched_pr(level); 
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);

  state = activated;

  //Activate current head of active_queue (if any) only when there are any new jobs (since the current job might have to be preempted
  sem_wait(&newjob_sem);
  //If there are no new jobs pending
  if(newjob_queue.size() == 0) {
    //activate current head of queue
    if(active_queue[0] != NULL) {
      active_queue[0]->activate();
    }
  }
  sem_post(&newjob_sem); 
  
    
  sem_post(&activation_sem);
}

///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void EDF::deactivate() {
  if(state == deactivated) {
    cout << "EDF::deactivate error - already deactivated!\n";
  }

  sem_wait(&activation_sem);
  sem_wait(&schedule_sem);

  //Deactivate currently active job (if any)
  if(active_queue[0] != NULL) {
    active_queue[0]->deactivate();
  }

  //Decrease the priority
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr();
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);  

  state = deactivated;

  sem_post(&sched_sem);
  sem_post(&activation_sem);
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution. This is executed by the worker thread itself (always of a lower priority than its scheduler)
void EDF::new_job(Runable* obj) {
  //Add new arrival to newjob_queue
  sem_wait(&newjob_sem);
  newjob_queue.push_back(obj);
  sem_post(&newjob_sem); 

  //Verify if current head has earlier deadline
  if(active_queue.size() > 0) {
    //If current head is earlier, no need to register event
    if (active_queue[0] < obj->getCriteria()->getDeadline()) {
      return;
    }
  }

  //If new job is the new nead, must update everything

  //Update object's schedulable criteria 
  criteria->setDeadline(obj->getCriteria()->getDeadline());
  
  //Notify parent of new head
  parent->new_job(this);

  //Alert scheduler of event 
  sem_wait(&sched_sem);
  sem_post(&event_sem);
  sem_post(&sched_sem);
  //sched_sem assures one event handled per post in the scheduler, otherwise, multiple jobs could be handled from just one 
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void EDF::renew_job(int runnable_id) {

}

///This function handles the end of a job in its load. It will always result in a change of executing thread. This is executed by the worker thread itself (always of a lower priority than its scheduler)
void EDF::job_finished(unsigned int runnable_id) {

  //Add new finish to finishedjob_queue
  sem_wait(&finishedjob_sem);
  finishedjob_queue.push_back(runnable_id);
  sem_post(&finishedjob_sem);

  //Alert parent of even
  parent->job_finished(id);

  //At this point, current object might loose priority, 

  //Alert current object of event
  sem_wait(&sched_sem);
  sem_post(&event_sem);
  sem_post(&sched_sem);
}

