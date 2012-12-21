#ifndef _EXCLUSIVE_CRITERIA_H
#define _EXCLUSIVE_CRITERIA_H

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class ExclusiveCriteria : public Criteria {
  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs nothing
  ExclusiveCriteria();

/*********** INHERITED FUNCTIONS ***********/

  ///This function sets a new deadline
  void setDeadline(struct timespec dln);
  
  ///This function sets the new priority
  void setPriority(int pr);

  ///This function returns the object's deadline (according to its criteria policy)
  struct timespec getDeadline();
  
  ///This function returns the object's priority (according to its criteria policy)
  int getPriority();
};

#endif
