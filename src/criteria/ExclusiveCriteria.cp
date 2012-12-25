#include "criteria/ExclusiveCriteria.h"

#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/
ExclusiveCriteria::ExclusiveCriteria() : Criteria() {
  //empty
}


/*********** INHERITED FUNCTIONS ***********/

///This function sets a new deadline
void ExclusiveCriteria::setDeadline(struct timespec dln) {
  if(priority == -1) {
    deadline = dln;
  }
  else {
    cout << "ExclusiveCriteria::setDeadline() error - object already has a priority assigned to it!\n";
  }
}

///This function sets the new priority
void ExclusiveCriteria::setPriority(int pr) {
  if ( deadline == TimeUtil::Millis(0) ) {
    if( priority >= 0 &&  priority < 100) {
      priority = pr;
    }
    else {
      cout << "ExclusiveCriteria::setPriority error - wrong priority value!\n";
    }
  }
  else {
    cout << "ExclusiveCriteria::setPriority() error - object already has a deadline assigned to it!\n";
  }
}

///This function returns the object's deadline (according to its criteria policy)
struct timespec ExclusiveCriteria::getDeadline() {
  if (deadline == TimeUtil::Millis(0)) {
    cout << "ExclusiveCriteria::getDeadline() error - object has no deadline assigned to it!\n";
  }

  return deadline;
}
  
///This function returns the object's priority (according to its criteria policy)
int ExclusiveCriteria::getPriority() {
  if (priority == -1) {
    cout << "ExclusiveCriteria::getDeadline() error - object has a priority assigned to it!\n";
  }

  return priority;
}
