#include "core/Task.h"
#include "core/Dispatcher.h"
#include "core/Simulation.h"
#include "util/TimeUtil.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/

Task::Task(Simulation *s, Dispatcher *disp, _task_load load) {
  sim = s;
  dispatcher = disp;
  task_load = load;
}

/*********** MEMBER FUNCTIONS ***********/

///This function performs one task (whether it's decoding one frame, or one busy-wait loop)
void Task::fire() {
  //empty
  cout << "Task::fire - This should not print!\n";
}
