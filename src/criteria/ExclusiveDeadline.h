#ifndef _EXCLUSIVE_DEADLINE_H
#define _EXCLUSIVE_DEADLINE_H

#include "core/Criteria.h"

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class ExclusiveDeadline : public Criteria {
  /*********** VARIABLES ***********/
 private:
  struct timespec deadline;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs nothing
  ExclusiveDeadline();

/*********** INHERITED FUNCTIONS ***********/

  ///This function sets a new deadline
  void setDeadline(struct timespec dln);
  
  ///This function sets the new priority
  void setPriority(int pr);

  ///This function returns the object's deadline (according to its criteria policy)
  struct timespec getDeadline();
  
  ///This function prints an alert.
  int getPriority();
};

#endif
