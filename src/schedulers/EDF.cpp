#include "schedulers/EDF.h"

#include "core/Criteria.h"
#include "queues/DeadlineQueue.h"
#include "util/Operators.h"

#include <iostream>

#define _INFO 0
#define _DEBUG 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
EDF::EDF(unsigned int _id, int level) : EventBased(_id, level) {
  #if _INFO==1
  cout << "Creating EDF with ID: " << id << endl;
  #endif

  //Create activeQueue object
  activeQueue = (RunnableQueue*) new DeadlineQueue();
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM EVENTBASED ****/

///This function compares a Criteria object with the current head of the active queue to determine whether the new Criteria is "greater". If newCriteria is "greater" this function returns true -> this could mean a higher priority, or earlier deadline, as defined by each subclass of the EventBased class. If this function returns true, the new arrival should be registered in order to update the activeQueue
bool EDF::greaterCriteria(Criteria* newCriteria) {
  //if there is a head, determine wheter the new criteria has an earlier deadline
  if(activeQueue->getSize() > 0) {
    struct timespec newDeadline = newCriteria->getDeadline();
    struct timespec currentDeadline = activeQueue->peek_front()->getCriteria()->getDeadline();
    
    //if newDeadline is earlier, the arrival must be registered with the scheduler to change the order of execution -> return true
    if( currentDeadline > newDeadline) {
      return true;
    }
    else {
      return false;
    }
  }
  //If the activeQueue is empty, event must be regitered
  else {
    return true;
  }
}
