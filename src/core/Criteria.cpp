#include "core/Criteria.h"

#include "util/TimeUtil.h"

#include <iostream>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/
Criteria::Criteria() {
  arrivalTime = TimeUtil::Millis(0);
  deadline = TimeUtil::Millis(0);
  period = TimeUtil::Millis(0);
  priority = -1;
}

/*********** GETTERS AND SETTERS ***********/

///This function sets a new arrivalTime
void Criteria::setArrivalTime(struct timespec at) {
  cout << "Criteria::setArrivalTime() - this should not print!\n";
}

///This function sets a new deadline
void Criteria::setDeadline(struct timespec dln) {
  cout << "Criteria::setDeadline() - this should not print!\n";
}

///This function sets a new period
void Criteria::setPeriod(struct timespec p) {
  cout << "Criteria::setPeriod() - this should not print!\n";  
}

///This function sets the new priority
void Criteria::setPriority(int pr) {
  cout << "Criteria::setPriority() - this should not print!\n";
}

///This function returns the current job's arrivalTime (according to its criteria policy)
struct timespec Criteria::getArrivalTime() {
  cout << "Criteria::getArrivalTime() - this should not print!\n";
  return arrivalTime;
}

///This function returns the object's deadline (according to its criteria policy)
struct timespec Criteria::getDeadline() {
  cout << "Criteria::getDeadline() - this should not print!\n";
  return deadline;
}

///This function returns the current job's period (according to its criteria policy)
struct timespec Criteria::getPeriod() {
  cout << "Criteria::getPeriod() - this should not print!\n";
  return period;  
}
  
///This function returns the object's priority (according to its criteria policy)
int Criteria::getPriority() {
  cout << "Criteria::getPriority() - this should not print!\n";
  return priority;
}
