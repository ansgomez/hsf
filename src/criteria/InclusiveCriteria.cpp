#include "criteria/InclusiveCriteria.h"

///This function sets a new deadline
void InclusiveCriteria::setDeadline(struct timespec dln) {
  deadline = dln;
}
  
///This function sets the new priority
void InclusiveCriteria::setPriority(int pr) {
  priority = pr;
}

///This function returns the object's deadline (according to its criteria policy)
struct timespec InclusiveCriteria::getDeadline() {
  return deadline;
}
  
///This function prints an alert.
int InclusiveCriteria::getPriority() { 
  return priority;
}
