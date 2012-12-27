/*********************************
 *
 * Input parameter 1    - file prefix
 * Input parameter 2..N - metric to compute
 *
 * Metrics include:
 *
 * execution_times              (exec)
 * response_times               (resp)
 * utilization                  (util)
 * resource_allocation_cost     (alloc)
 * system_implementation_cost   (sys)
 * -----------------------------------
 * All of the above             (all)
 *
 *********************************/

#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

/*********** VARIABLES ***********/

///This variable holds the prefix for the input files (_traces, _runtimes, _missedDeadlines).
string prefix="simulation";

/*********** FUNCTIONS ***********/

///This function calls the show_execution_times script to produce "$prefix_execution_times.csv"
void showExecutionTimes();

///This function calls the show_response_times script to produce "$prefix_response_times.csv"
void showResponseTimes();

///This function calls the show_Utilization script to produce "$prefix_Utilization.csv"
void showUtilization();

///This function calls the Resource Allocation Cost script to produce "$prefix_alloc_cost_us.csv"
void  showResourceAllocationCost();


///This function calls the showSystemCost script to show "$prefix_System Cost"
void  showSystemCost();

///This function calls the showWorkerCost script to show "$prefix_Worker Cost"
void  showWorkerCost();

///This function interprets all input parameters
void interpret(char *str);

/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {

  cout << "Starting HSF's show tool\n";

  ///If there are input pArameters, interpret them
  if(argn > 1) {
    for(int i=1;i<=argn;i++) {
      interpret(argv[i]);
    }
  }
  ///otherwise analyze all by default, 
  else {
    cout << "Showing all...\n";
    showExecutionTimes();
    showResponseTimes();
    showUtilization();
    showResourceAllocationCost();
    showSystemCost();
    showWorkerCost();
  }

  cout << "\nDone." <<endl;

  return 0;
}


///This function calls the show_execution_times script to show "$prefix_execution_times"
void showExecutionTimes() {
  string cmd = "octave --no-window-system -q --eval \"show_execution_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the show_response_times sCript to show "$prefix_response_times"
void showResponseTimes() {
  string cmd = "octave --no-window-system -q --eval \"show_response_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the show_utilization script to show "$prefix_utilization"
void showUtilization() {
  string cmd = "octave --no-window-system -q --eval \"show_utilization('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showResourceAllocationCost script to show "$prefix_Resource Allocation Cost"
void  showResourceAllocationCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_resourceAllocationCost('" + prefix + "')\"";

  system(cmd.c_str());
}


///This function calls the showSystemCost script to show "$prefix_System Cost"
void  showSystemCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_systemCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showWorkerCost script to show "$prefix_Worker Cost"
void  showWorkerCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_workerCost('" + prefix + "')\"";

  system(cmd.c_str());
}
///This function interprets all input parameters
void interpret(char *str) {
  //empty
}
