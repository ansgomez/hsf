#include "schedulers/EDF.h"

#include "core/Simulation.h"
#include "core/Criteria.h"
#include "core/Runnable.h"
#include "core/RunnableQueue.h"
#include "core/Scheduler.h"
#include "pthread/Priorities.h"
#include "queues/DeadlineQueue.h"
#include "results/Trace.h"
#include "util/Enumerations.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>
#include <time.h>

#define _INFO 0
#define _DEBUG 0

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
EDF::EDF(unsigned int _id, int level) : Scheduler(_id, level) {
#if _INFO==1
  cout << "Creating EDF with ID: " << id << endl;
#endif

  //Create activeQueue object
  activeQueue = (RunnableQueue*) new DeadlineQueue();

  //Initialize semaphores
  sem_init(&activation_sem, 0, 1);  //mutex semaphore
  sem_init(&event_sem, 0, 0);       //sem used as signal
  sem_init(&jobfinished_sem, 0, 1); //mutex semaphore
  sem_init(&newjob_sem, 0, 1);      //mutex semaphore
  sem_init(&schedule_sem, 0, 1);    //mutex semaphore
}

//TODO: DESTRUCTOR

/*********** INHERITED FUNCTIONS ***********/

/**** FROM RUNNABLE  ****/

///This function rewrites the activate method to 'activate' both the scheduler as well as its load - this runs in the higher level scheduler thread
void EDF::activate() {

#if _DEBUG==1
  cout << "Activating edf...\n";
#endif

  if(state == activated) {
    cout << "EDF::activate error - already activated!\n";
  }

  sem_wait(&activation_sem);
    setPriority(Priorities::get_sched_pr(level));
    state = activated;

    //Activate current head of active_queue (if any) only when there are 
    //any new jobs (since the current job might have to be preempted)
    sem_wait(&newjob_sem);
      //If there are no new jobs pending
      if(newJobQueue.size() == 0) {
        //activate current head of queue
	if(activeQueue == NULL)
	  cout << "EDF:activate activeQueue is null!\n";

        if(activeQueue->peek_front() != NULL) {
          activeQueue->peek_front()->activate();
        }
      }

      //else, wait for scheduler to execute and process the new jobs himself
    sem_post(&newjob_sem); 
  sem_post(&activation_sem);

#if _DEBUG==1
  cout << "Activated edf\n";
#endif
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

#if _DEBUG==1
  cout << "job_finished edf\n";
#endif

  //Add id to jobfinishedQueue
  sem_wait(&jobfinished_sem);
  jobFinishedQueue.push_back(runnable_id);
  sem_post(&jobfinished_sem);

  //Register event with parent allocator
  if(parent!=NULL) {
    parent->job_finished(id);
  }
  else if (level != 0) {
    cout << "EDF::job_finished - non-top level entity has null parent!\n";
  }

  //At this point, current object might loose priority, 

  //Register event with this scheduler
  sem_wait(&schedule_sem);
    sem_post(&event_sem); //->posting to event_sem must be protected by sched_sem!
  sem_post(&schedule_sem);
  //Protecting posts to event_sem assures one event handled per post in the scheduler, otherwise, multiple jobs could be handled from just one post
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution. This is executed by the worker thread itself (always of a lower priority than its scheduler)
void EDF::new_job(Runnable* obj) {

#if _DEBUG==1
  cout << "new_job edf\n";
#endif

  //Add new arrival to newjob_queue
  sem_wait(&newjob_sem);
    newJobQueue.push_back(obj);
  sem_post(&newjob_sem); 

  if(newJobQueue.size()>1)
    return;

  //Verify if current head (if any) has earlier deadline
  if(activeQueue->getSize() > 0) {
    //If current head's deadline is earlier, no need to register event with scheduler (possibly it "wake-up")
    if (activeQueue->peek_front()->getCriteria()->getDeadline() < obj->getCriteria()->getDeadline()) {
      return;
    }
    else {
      cout << "EDF::new_job - there will be a new head...\n";
    }
  }

  cout << "registering new head...\n";

  //Set object's schedulable criteria 
  criteria = obj->getCriteria();
 
  //Notify parent of new head
  if(parent!=NULL)
    parent->new_job(this);

  //Alert scheduler of event 
  sem_wait(&schedule_sem);
    sem_post(&event_sem); //->posting to event_sem must be protected by sched_sem!
  sem_post(&schedule_sem);
  //Protecting posts to event_sem assures one event handled per post in the scheduler, otherwise, multiple jobs could be handled from just one post
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void EDF::renew_job(Runnable* r) {
  //todo
}

/**** FROM SCHEDULER  ****/

///This function performs the actual scheduling (figuring out the order of execution for its load)
void EDF::schedule() {

#if _DEBUG==1
  cout << "schedule edf\n";
#endif

  Runnable* currentRunnable;

  while(Simulation::isSimulating()) {

    //Wait for an event to ocurr
    sem_wait(&event_sem);
    //TODO: make sure that event_sem isn't posted more than once (unless necessary)

    sem_wait(&schedule_sem);
    currentRunnable = activeQueue->peek_front();

    //Deactivate currently active job (if any) in order to process new/finished jobs
    if(currentRunnable != NULL) {
      currentRunnable->deactivate();
    }

    //Add every new job to the activeQueue
    sem_wait(&newjob_sem);
      while(newJobQueue.size() > 0) {
        activeQueue->insertRunnable(newJobQueue.front());//insert head of newJobQueue
        newJobQueue.pop_front();//erase head of newJobQueue
      }
    sem_post(&newjob_sem);

    //Remove any finished jobs from the active queue
    sem_wait(&jobfinished_sem);
      while(jobFinishedQueue.size() > 0) {
        activeQueue->deleteRunnable(jobFinishedQueue.front());//erase from RunnableQueue
	jobFinishedQueue.pop_front();//erase from jobFinishedQueue
      }
    sem_post(&jobfinished_sem);

    //If activeQueue is empty, release sched_sem, and wait for next event
    if(activeQueue->getSize() == 0) {
      sem_post(&schedule_sem);
      continue;
    }

    //Otherwise, activate head of activeQueue
    activeQueue->peek_front()->activate();

    //Release sched_sem
    sem_post(&schedule_sem);
  }
}

