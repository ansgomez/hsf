#ifndef _TDMA_H
#define _TDMA_H

#include "Scheduler.h"
#include "Simulation.h"

#include <semaphore.h>

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class TDMA : public Scheduler {
 private:
  vector<struct timespec> timeslots; //There should be one timeslot per load

  sem_t schedule_sem, preempt_sem;
  int timing;

 public:
  /*********** CONSTRUCTOR ***********/
  ///Constructor needs pointer to simulation
  TDMA(Simulation *s, unsigned int id);

  /*********** INHERITED FUNCTIONS ***********/
  ///This function performs the actual scheduling (figuring out the order of execution for its load)
  void schedule();

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  void new_job(Worker * worker);

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  void job_finished(int worker);

  ///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load
  void activate();

  ///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
  void deactivate();

  /*********** MEMBER FUNCTIONS ***********/
  ///This function adds a slot to the TDMA scheduler
  void add_slot(struct timespec slot);
};
#endif
