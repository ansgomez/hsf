#ifndef _INTERMEDIARY_H
#define _INTERMEDIARY_H

#include "core/Worker.h"
#include "core/Runnable.h"
#include "util/Enumerations.h"
#include <vector>

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Intermediary : public Runnable {

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs runnable's id
  Intermediary(unsigned int _id);

  /*********** INHERITED FUNCTIONS ***********/

  ///This is the pthread's wrapper function
  void wrapper();

  ///This function rewrites the activate method to activate both the scheduler as well as its load
  virtual void activate();

  ///This function rewrites the deactivate method both the scheduler as well as its load
  virtual void deactivate();

  ///This function rewrites the join method to account for the scheduler's load (they are all joined)
  virtual void join();

  /*********** MEMBER FUNCTIONS ***********/
  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void new_job(Runnable *ojb);

  ///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
  virtual void renew_job(Runnable* r);

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void job_finished(int runnable_id);
};
#endif
