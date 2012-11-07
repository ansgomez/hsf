#ifndef _EDF_H
#define _EDF_H


#include "Scheduler.h"

class Deadline;

class EDF : public Scheduler {
 private:
  ///These queues are used to hold runnables with active jobs, or those with jobs pending
  Vector<Runnable*> active_queue, newjob_queue;
  
  ///This queue holds the ids of runnables in the active queue who already finished
  Vector<unsigned int> finishedjob_sem;

  ///Semaphores to ensure proper execution
  sem_t schedule_sem, event_sem, newjob_sem, jobfinished_sem, activation_sem;

  /*********** CONSTRUCTOR ***********/
 public:  
  ///Constructor needs pointer to simulation, id, and its level
  EDF(Simulation *s, unsigned int id, int level);

  /*********** INHERITED FUNCTIONS ***********/
  ///This function performs the actual scheduling (figuring out the order of execution for its load)
  void schedule();

  ///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
  void activate();

  ///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
  void deactivate();

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  void new_job(Runnable* obj);

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  void job_finished(unsigned int runnable_id);

};
#endif
