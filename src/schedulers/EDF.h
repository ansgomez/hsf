#ifndef _EDF_H
#define _EDF_H

#include "core/Scheduler.h"

#include <deque>

class RunnableQueue;

using namespace std;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class EDF : public Scheduler {

 private:

  ///This queue holds all active Runnables underneath this scheduler.
  RunnableQueue* activeQueue;

  ///This queue holds the ids of runnables in the active queue who already finished
  deque<unsigned int> jobFinishedQueue;

  ///This queue holds the Runnables who are registering new jobs
  deque<Runnable*> newJobQueue;

  ///Semaphores to ensure proper execution
  sem_t activation_sem, event_sem, jobfinished_sem, newjob_sem, schedule_sem;

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  EDF(unsigned int id, int level);

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM THREAD  ****/

  ///This function redefines Thread::join() to take into account EDF unblocking mechanism...
  void join();

  /**** FROM RUNNABLE  ****/
  
  ///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
  void activate();

  ///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
  void deactivate();

  /**** FROM INTERMEDIARY  ****/
  
  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  void job_finished(unsigned int runnable_id);

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  void new_job(Runnable *ojb);

  ///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
  void renew_job(Runnable* r);

  /**** FROM SCHEDULER  ****/
  
  ///This function performs the actual scheduling (figuring out the order of execution for its load)
  void schedule();
};

#endif
