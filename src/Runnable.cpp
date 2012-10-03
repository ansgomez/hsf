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

Runnable::Runnable(Simulation *s, unsigned int _id) : Thread(s,_id) {
  sim = s;
}

///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
void Runnable::activate() {
  cout << "Runnable::activate - This should not print!\n";
}

///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
void Runnable::deactivate() {
  cout << "Runnable::deactivate - This should not print!\n";
}

void Runnable::wrapper() {
  //empty
  cout << "Runnable::wrapper - This should not print!\n";
}
