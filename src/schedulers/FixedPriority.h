#ifndef _FIXEDPRIORITY_H
#define _FIXEDPRIORITY_H

#include "schedulers/EventBased.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class FixedPriority : public EventBased {

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  FixedPriority(unsigned int id, int level);

  /*********** INHERITED FUNCTIONS ***********/

  ///This function compares a Criteria object with the current head of the active queue to determine whether the new Criteria is "greater" -> this could mean a higher priority, or earlier deadline, as defined by each subclass of the EventBased class.
  bool greaterCriteria(Criteria* newCriteria);

};

#endif
