#ifndef _CRITERIA_H
#define _CRITERIA_H

#include <time.h>

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Criteria {
  /*********** VARIABLES ***********/
 protected:
  ///Deadline can be used as a criteria for scheduling
  struct timespec deadline;
  
  ///Priority can also be used as a criteria for scheduling
  int priority;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs nothing
  Criteria();
  
  /*********** GETTERS AND SETTERS ***********/
  ///This function sets a new deadline
  virtual void setDeadline(struct timespec dln);

  ///This function sets the new priority
  virtual void setPriority(int pr);

  ///This function returns the object's deadline (according to its criteria policy)
  virtual struct timespec getDeadline();
  
  ///This function returns the object's priority (according to its criteria policy)
  virtual int getPriority();
};

#endif
