#include "Task.h"
#include "Dispatcher.h"
#include "Simulation.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Task::Task(Simulation *s, Dispatcher *disp, _task_load load) {
  sim = s;
  dispatcher = disp;
  task_load = load;
}

void Task::fire() {
  //empty
  cout << "Task::fire - This should not print!\n";
}
