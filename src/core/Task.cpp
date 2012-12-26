#include "core/Task.h"

#include "core/Dispatcher.h"
#include "util/TimeUtil.h"

#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

Task::Task(Dispatcher *disp, _task_load load) {
  dispatcher = disp;
  task_load = load;
}

/*********** MEMBER FUNCTIONS ***********/

///This function performs one task (whether it's decoding one frame, or one busy-wait loop)
void Task::fire() {
  //empty
  cout << "Task::fire - This should not print!\n";
}
