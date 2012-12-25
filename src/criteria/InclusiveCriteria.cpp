#include "criteria/InclusiveCriteria.h"

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

InclusiveCriteria::InclusiveCriteria() : Criteria() {
  //empty
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM CRITERIA ****/

///This function returns the current job's arrivalTime (according to its criteria policy)
struct timespec InclusiveCriteria::getArrivalTime() {
  return arrivalTime;
}

///This function returns the object's deadline (according to its criteria policy)
struct timespec InclusiveCriteria::getDeadline() {
  return deadline;
}
  
///This function returns the current job's period (according to its criteria policy)
struct timespec InclusiveCriteria::getPeriod() {
  return period;
}

///This function prints an alert.
int InclusiveCriteria::getPriority() { 
  return priority;
}

///This function sets a new arrivalTime
void InclusiveCriteria::setArrivalTime(struct timespec at) {
  arrivalTime = at;
}

///This function sets a new deadline
void InclusiveCriteria::setDeadline(struct timespec dln) {
  deadline = dln;
}
  
///This function sets a new period
void InclusiveCriteria::setPeriod(struct timespec p) {
  period = p;
}

///This function sets the new priority
void InclusiveCriteria::setPriority(int pr) {
  priority = pr;
}
