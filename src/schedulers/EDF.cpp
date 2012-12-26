#include "schedulers/EDF.h"

#include "core/Simulation.h"
#include "core/Criteria.h"
#include "core/Runnable.h"
#include "core/RunnableQueue.h"
#include "core/Scheduler.h"
#include "pthread/Priorities.h"
#include "results/Trace.h"
#include "util/Enumerations.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <time.h>
#include <iostream>

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
EDF::EDF(unsigned int _id, int level) : Scheduler(_id, level) {
#if _INFO==1
  cout << "Creating EDF with ID: " << id << endl;
#endif

  //TODO: Check initialization
  sem_init(&schedule_sem, 0, 1); //mutex semaphore
  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&newjob_sem, 0, 1); //mutex semaphore
  sem_init(&jobfinished_sem, 0, 1); //mutex semaphore
  sem_init(&preempt_sem, 0, 0); //sem used as signal
}

//TODO: DESTRUCTOR

/*********** INHERITED FUNCTIONS ***********/

/**** FROM RUNNABLE  ****/


///This function rewrites the activate method to 'activate' both the scheduler as well as its load - this runs in the higher level scheduler thread
void EDF::activate() {

  if(state == activated) {
    cout << "EDF::activate error - already activated!\n";
  }

  sem_wait(&activation_sem);
    
  setPriority(Priorities::get_sched_pr(level));

  state = activated;

  //Activate current head of active_queue (if any) only when there are any new jobs (since the current job might have to be preempted
  sem_wait(&newjob_sem);
  //If there are no new jobs pending
  if(newJobQueue->getSize() == 0) {
    //activate current head of queue
    if(activeQueue->peek_front() != NULL) {
      activeQueue->peek_front()->activate();
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
  if(activeQueue->peek_front() != NULL) {
    activeQueue->peek_front()->deactivate();
  }

  //Decrease the priority
  setPriority(Priorities::get_inactive_pr());

  state = deactivated;

  sem_post(&schedule_sem);
  sem_post(&activation_sem);
}

/**** FROM INTERMEDIARY  ****/

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void EDF::job_finished(unsigned int runnable_id) {

  //Add new finish to jobfinished_queue
  sem_wait(&jobfinished_sem);
  jobFinishedQueue.push_back(runnable_id);
  sem_post(&jobfinished_sem);

  //Alert parent of even
  parent->job_finished(id);

  //At this point, current object might loose priority, 

  //Alert current object of event
  sem_wait(&schedule_sem);
  sem_post(&event_sem);
  sem_post(&schedule_sem);
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution. This is executed by the worker thread itself (always of a lower priority than its scheduler)
void EDF::new_job(Runnable* obj) {
  //Add new arrival to newjob_queue
  sem_wait(&newjob_sem);
  newJobQueue->insertRunnable(obj);
  sem_post(&newjob_sem); 

  //Verify if current head has earlier deadline
  if(activeQueue->getSize() > 0) {
    //If current head is earlier, no need to register event
    if (activeQueue->peek_front()->getCriteria()->getDeadline() < obj->getCriteria()->getDeadline()) {
      return;
    }
  }

  //If new job is the new nead, must update everything

  //Update object's schedulable criteria 
  criteria->setDeadline(obj->getCriteria()->getDeadline());
  
  //Notify parent of new head
  parent->new_job(this);

  //Alert scheduler of event 
  sem_wait(&schedule_sem);
  sem_post(&event_sem);
  sem_post(&schedule_sem);
  //sched_sem assures one event handled per post in the scheduler, otherwise, multiple jobs could be handled from just one 
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void EDF::renew_job(Runnable* r) {

}

/**** FROM SCHEDULER  ****/

///This function performs the actual scheduling (figuring out the order of execution for its load)
void EDF::schedule() {

  while(Simulation::isSimulating()) {
    //Wait for an event to ocurr
    sem_wait(&event_sem);
    //todo make sure that event_sem isn't posted more than once (unless necessary)

    sem_wait(&schedule_sem);

    Runnable *currentJob = activeQueue->peek_front();
    
    //Deactivate currently active job (if any)
    if(currentJob != NULL) {
      currentJob->deactivate();
    }

    //Add any new jobs to the active queue
    sem_wait(&newjob_sem);
    while(newJobQueue->getSize() > 0) {
      activeQueue->insertRunnable(newJobQueue->pop_front());
    }
    sem_post(&newjob_sem);

    //Remove any finished jobs from the active queue
    sem_wait(&jobfinished_sem);
    while(jobFinishedQueue.size() > 0) {
      //erase from RunnableQueue
      activeQueue->deleteRunnable(jobFinishedQueue[0]);
      //erase from jobFinishedQueue
      jobFinishedQueue.erase(jobFinishedQueue.begin());
    }
    sem_post(&jobfinished_sem);

    //If activeQueue is empty, wait for next event
    if(activeQueue->getSize() == 0) {
      continue;
    }

    //Otherwise, activate head of activeQueue and post to schedule sem to register changes with scheduler thread
    activeQueue->peek_front()->activate();

    sem_post(&schedule_sem);
  }
}

