#ifndef _RM_H
#define _RM_H

#include "schedulers/EventBased.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class RateMonotonic : public EventBased {

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  RateMonotonic(unsigned int id, int level);

  /*********** INHERITED FUNCTIONS ***********/

  ///This function compares a Criteria object with the current head of the active queue to determine whether the new Criteria is "greater" -> this could mean a higher priority, or earlier deadline, as defined by each subclass of the EventBased class.
  bool greaterCriteria(Criteria* newCriteria);

};

#endif
