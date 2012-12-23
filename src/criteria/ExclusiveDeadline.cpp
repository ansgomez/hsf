#include "criteria/ExclusiveDeadline.h"

///This function sets a new deadline
void ExclusiveDeadline::setDeadline(struct timespec dln) {
  deadline = dln;
}
  
///This function sets the new priority
void ExclusiveDeadline::setPriority(int pr) {
 cout << "ExclusiveDeadline error: attempting setPriority(int)\n";
}

///This function returns the object's deadline (according to its criteria policy)
struct timespec ExclusiveDeadline::getDeadline() {
  return deadline;
}
  
///This function prints an alert.
int ExclusiveDeadline::getPriority() { 
  cout << "ExclusiveDeadline error: attempting getPriority()\n";

  return -1;
}
