#include "schedulers/RateMonotonic.h"

#include "core/Criteria.h"
#include "queues/PeriodQueue.h"
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
RateMonotonic::RateMonotonic(unsigned int _id, int level) : EventBased(_id, level) {
  #if _INFO==1
  cout << "Creating RateMonotonic with ID: " << id << endl;
  #endif

  //Create activeQueue object
  activeQueue = (RunnableQueue*) new PeriodQueue();
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM EVENTBASED ****/

///This function compares a Criteria object with the current head of the active queue to determine whether the new Criteria is "greater" -> this could mean a higher priority, or earlier deadline, as defined by each subclass of the EventBased class.
bool RateMonotonic::greaterCriteria(Criteria* newCriteria) {
  //if there is a head, determine wheter the new criteria has an earlier deadline
  if(activeQueue->getSize() > 0) {
    
    struct timespec newPeriod = newCriteria->getPeriod();
    struct timespec currentPeriod= activeQueue->peek_front()->getCriteria()->getPeriod();
    
    //if newPeriod is smaller, the arrival must be registered with the scheduler to change the order of execution -> return true
    if( newPeriod < currentPeriod ) {
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

