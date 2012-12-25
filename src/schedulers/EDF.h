#ifndef _EDF_H
#define _EDF_H

//#include "core/RunnableQueue.h"
#include "core/Scheduler.h"

#include <vector>

class RunnableQueue;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class EDF : public Scheduler {

 private:

  ///This queue holds all active Runnables underneath this scheduler.
  RunnableQueue* activeQueue;

  ///These queues are used to hold runnables with active jobs, or those with jobs pending
  RunnableQueue* newJobQueue;
  
  ///This queue holds the ids of runnables in the active queue who already finished
  vector<unsigned int> jobFinishedQueue;

  ///Semaphores to ensure proper execution
  sem_t schedule_sem, preempt_sem, event_sem, newjob_sem, jobfinished_sem, activation_sem;

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  EDF(Simulation* s, unsigned int id, int level);

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM RUNNABLE  ****/
  
  ///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
  void activate();

  ///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
  void deactivate();

  /**** FROM INTERMEDIARY  ****/
  
  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  void new_job(Runnable *ojb);

  ///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
  void renew_job(Runnable* r);

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  void job_finished(unsigned int runnable_id);

  /**** FROM SCHEDULER  ****/
  
  ///This function performs the actual scheduling (figuring out the order of execution for its load)
  void schedule();
};

#endif
