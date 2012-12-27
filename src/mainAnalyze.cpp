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

///This function calls the execution_times script to produce "$prefix_execution_times.csv"
void analyzeExecutionTimes();

///This function interprets all input parameters
void interpret(char *str);

/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {

  cout << "Starting HSF's analyze tool\n";

  ///If there are input parameters, interpret them
  if(argn > 1) {
    for(int i=1;i<=argn;i++) {
      interpret(argv[i]);
    }
  }
  ///otherwise analyze all by default, 
  else {
    cout << "Analyzing all...\n";
    analyzeExecutionTimes();
  }

  cout << "\nDone." <<endl;

  return 0;
}


///This function calls the execution_times script to produce "$prefix_execution_times.csv"
void analyzeExecutionTimes() {
  string cmd = "octave --no-window-system -p $HSF/scripts/octave -qf --eval \"execution_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function interprets all input parameters
void interpret(char *str) {
  //empty
}
