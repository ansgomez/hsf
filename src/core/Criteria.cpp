#include "core/Criteria.h"

#include "util/TimeUtil.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/
Criteria::Criteria() {
  arrivalTime = Millis(0);
  deadline = Millis(0);
  period = Millis(0);
  priority = -1;
}

/*********** GETTERS AND SETTERS ***********/

///This function sets a new deadline
void Criteria::setDeadline(struct timespec dln) {
  cout << "Criteria::setDeadline() - this should not print!\n";
}

///This function sets the new priority
void Criteria::setPriority(int pr) {
  cout << "Criteria::setPriority() - this should not print!\n";
}

///This function returns the object's deadline (according to its criteria policy)
struct timespec Criteria::getDeadline() {
  cout << "Criteria::getDeadline() - this should not print!\n";
}
  
///This function returns the object's priority (according to its criteria policy)
int Criteria::getPriority() {
  cout << "Criteria::getPriority() - this should not print!\n";
}
