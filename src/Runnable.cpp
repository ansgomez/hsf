#include "Runnable.h"

#include "Task.h"
#include "Scheduler.h"
#include "Simulation.h"
#include "Priorities.h"
#include "Trace.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/

Runnable::Runnable(Simulation *s, unsigned int _id) : Thread(s,_id) {
  sim = s;
  state = deactivated;

  //TODO: parametrize this!
  criteria = (Criteria *) (new ExclusiveCriteria());
}

/*********** INHERITED FUNCTIONS ***********/

///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
void Runnable::activate() {
  cout << "Runnable::activate - This should not print!\n";
}

///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
void Runnable::deactivate() {
  cout << "Runnable::deactivate - This should not print!\n";
}

///This function will be executed by the pthread
void Runnable::wrapper() {
  //empty
  cout << "Runnable::wrapper - This should not print!\n";
}

///This function joins the calling thread with the object's pthread
void Runnable::join() {
  //empty
  cout << "Runnable::join - This should not print!\n";
}

/*********** GETTERS AND SETTERS ***********/
///This function return the runnable's schedulable criteria
Criteria* getCriteria() {
  return criteria;
}

///This function sets the runnable's parent
void setParent(Runnable *p) {
  parent = p;
}
