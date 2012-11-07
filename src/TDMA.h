#ifndef _TDMA_H
#define _TDMA_H

#include "Scheduler.h"
#include "Simulation.h"

#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class TDMA : public Scheduler {
  /*********** VARIABLES ***********/
 private:
  int active_index;

  ///This vector stores all of the runnables it controls (it's load)
  vector<Runnable*> load;

  ///There should be one timeslot per load
  vector<struct timespec> timeslots; 

  ///Semaphores to ensure proper execution
  sem_t schedule_sem, preempt_sem, timing_sem, activation_sem;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs pointer to simulation
  TDMA(Simulation *s, unsigned int id, int level);

  /*********** INHERITED FUNCTIONS ***********/
  ///This function performs the actual scheduling (figuring out the order of execution for its load)
  void schedule();

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  void new_job(Worker* worker);

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  void job_finished(unsigned int worker_id);

  ///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load
  void activate();

  ///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
  void deactivate();

  ///This function rewrites the join method to account for the scheduler's load (they are all joined)
  void join();

  /*********** MEMBER FUNCTIONS ***********/
  ///This function adds a load to the scheduler (could be another scheduler, or a worker)
  void add_load(Runnable *new_load);

  ///This function adds a slot to the TDMA scheduler
  void add_slot(struct timespec slot);
};
#endif
