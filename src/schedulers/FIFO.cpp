#include "schedulers/FIFO.h"

#include "core/Criteria.h"
#include "queues/FIFOQueue.h"
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
FIFO::FIFO(unsigned int _id, int level) : EventBased(_id, level) {
  #if _INFO==1
  cout << "Creating RateMonotonic with ID: " << id << endl;
  #endif

  //Create activeQueue object
  activeQueue = (RunnableQueue*) new FIFOQueue();
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM EVENTBASED ****/

///This function compares a Criteria object with the current head of the active queue to determine whether the new Criteria is "greater" -> this could mean a higher priority, or earlier deadline, as defined by each subclass of the EventBased class.
bool FIFO::greaterCriteria(Criteria* newCriteria) {
  //if there is a head, determine wheter the new criteria has an earlier deadline
  if(activeQueue->getSize() > 0) {
    
    struct timespec newArrivalTime = newCriteria->getArrivalTime();
    struct timespec currentArrivalTime= activeQueue->peek_front()->getCriteria()->getArrivalTime();
    
    //if newArrivalTime is earlier, the arrival must be registered with the scheduler to change the order of execution -> return true
    if( currentArrivalTime > newArrivalTime ) {
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

