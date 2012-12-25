#ifndef _INCLUSIVE_CRITERIA_H
#define _INCLUSIVE_CRITERIA_H

#include "core/Criteria.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/
 
class InclusiveCriteria : public Criteria {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs nothing
  InclusiveCriteria();

  /*********** INHERITED FUNCTIONS ***********/
  
  /**** FROM CRITERIA ****/
  
  ///This function returns the current job's arrivalTime (according to its criteria policy)
  struct timespec getArrivalTime();

  ///This function returns the current job's deadline (according to its criteria policy)
  struct timespec getDeadline();

  ///This function returns the current job's period (according to its criteria policy)
  struct timespec getPeriod();
  
  ///This function returns the object's priority (according to its criteria policy)
  int getPriority();

  ///This function sets a new arrivalTime
  void setArrivalTime(struct timespec at);

  ///This function sets a new deadline
  void setDeadline(struct timespec dln);

  ///This function sets a new period
  void setPeriod(struct timespec p);

  ///This function sets the new priority
  void setPriority(int pr);
};

#endif
