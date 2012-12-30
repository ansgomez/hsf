#ifndef _RESOURCE_ALLOCATOR_H
#define _RESOURCE_ALLOCATOR_H

#include "core/Runnable.h"
#include "util/Enumerations.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class ResourceAllocator : public Runnable {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs runnable's id
  ResourceAllocator(unsigned int _id);

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM THREAD ****/

  ///This is the pthread's wrapper function
  void wrapper();
  
  /**** FROM RUNNABLE ****/

  ///This function rewrites the activate method to activate both the scheduler as well as its load
  virtual void activate();

  ///This function rewrites the deactivate method both the scheduler as well as its load
  virtual void deactivate();

  /*********** MEMBER FUNCTIONS ***********/
  
  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void new_job(Runnable *r);

  ///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
  virtual void renew_job(Runnable* r);

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void job_finished(unsigned int id);
};
#endif
