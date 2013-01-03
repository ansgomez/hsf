#ifndef _EDF_H
#define _EDF_H

#include "schedulers/EventBased.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class EDF : public EventBased {

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  EDF(unsigned int id, int level);

  /*********** INHERITED FUNCTIONS ***********/

  ///This function compares a Criteria object with the current head of the active queue to determine whether the new Criteria is "greater". If newCriteria is "greater" this function returns true -> this could mean a higher priority, or earlier deadline, as defined by each subclass of the EventBased class. If this function returns true, the new arrival should be registered in order to update the activeQueue
  bool greaterCriteria(Criteria* newCriteria);

};

#endif
