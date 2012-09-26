#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "Worker.h"
#include "Runnable.h"
#include "Enumerations.h"
#include <vector>

using namespace std;

class Simulation;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Scheduler : public Runnable {
 protected:
  Simulation *sim;

  _sched_type sched_type;

  vector<Runnable*> load;

  ///This variable holds the index of the currently active load
  unsigned int active_index;

 public:
  /*********** CONSTRUCTOR ***********/

  ///Constructor needs pointer to simulation
  Scheduler(Simulation *s, unsigned int _id);

  /*********** INHERITED FUNCTIONS ***********/

  ///This is the pthread's wrapper function
  void wrapper();

  ///This function rewrites the deactivate method both the scheduler as well as its load
  void deactivate();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function adds a load to the scheduler (could be another scheduler, or a worker)
  void add_load(Runnable *new_load);

  ///This function performs the actual scheduling (figuring out the order of execution for its load)
  virtual void schedule();

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void new_job(Worker *worker);

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void job_finished(int worker);

  ///This function waits for the scheduler's load to join
  void join_all();
};
#endif
