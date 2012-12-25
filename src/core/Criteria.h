#ifndef _CRITERIA_H
#define _CRITERIA_H

#include <time.h>

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Criteria {

 protected:
 
  /*********** VARIABLES ***********/
 
  ///This variable holds the arrival time of the Runnable's current job
  struct timespec arrivalTime;

  ///This variable holds the runnable's absolute deadline
  struct timespec deadline;
  
  ///This variable holds the period of the Runnable's current job
  struct timespec period;

  ///This variable holds the runnable's priority
  int priority;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs nothing
  Criteria();
  
  /*********** GETTERS AND SETTERS ***********/

  ///This function returns the current job's arrivalTime (according to its criteria policy)
  virtual struct timespec getArrivalTime();

  ///This function returns the current job's deadline (according to its criteria policy)
  virtual struct timespec getDeadline();

  ///This function returns the current job's period (according to its criteria policy)
  virtual struct timespec getPeriod();
  
  ///This function returns the object's priority (according to its criteria policy)
  virtual int getPriority();

  ///This function sets a new arrivalTime
  virtual void setArrivalTime(struct timespec at);

  ///This function sets a new deadline
  virtual void setDeadline(struct timespec dln);

  ///This function sets a new period
  virtual void setPeriod(struct timespec p);

  ///This function sets the new priority
  virtual void setPriority(int pr);
};

#endif
