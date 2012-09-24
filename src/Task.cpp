#include "Task.h"
#include "Dispatcher.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Task::Task(Dispatcher *disp, _task_load load) {
  dispatcher = disp;
  task_load = load;
}

void Task::fire() {
  //empty
  cout << "Task::fire - This should not print!\n";
}
