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

///This function calls the response_times script to produce "$prefix_response_times.csv"
void analyzeResponseTimes();

///This function calls the Utilization script to produce "$prefix_Utilization.csv"
void analyzeUtilization();

///This function calls the Resource Allocation Cost script to produce "$prefix_alloc_cost_us.csv"
void  analyzeResourceAllocationCost();

///This function calls the analyzeSystemCost script to show "$prefix_System Cost"
void  analyzeSystemCost();

////This function calls the analyzeWorkerCost script to show "$prefix_Worker Cost"
void analyzeWorkerCost();

///This function interprets all input parameters
void interpret(char *str);

/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {

#if _INFO==1
  cout << "Starting HSF's analyze tool\n";
#endif

  ///If there are input parameters, interpret them
  if(argn > 1) {
    for(int i=1;i<=argn;i++) {
      interpret(argv[i]);
    }
  }
  ///otherwise analyze all by default, 
  else {
    cout << "\n***   Analyzing all metrics!\t***\n\n";
    analyzeExecutionTimes();
    analyzeResponseTimes();
    analyzeUtilization();
    analyzeResourceAllocationCost();
    analyzeSystemCost();
    analyzeWorkerCost();
  }

#if _INFO==1
  cout << "\nDone." <<endl;
#endif

  return 0;
}


///This function calls the execution_times script to produce "$prefix_execution_times.csv"
void analyzeExecutionTimes() {
  cout << "Calculating Execution Times...\n";

  string cmd = "octave --no-window-system -q --eval \"execution_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the response_times script to produce "$prefix_response_times.csv"
void analyzeResponseTimes() {
  cout << "Calculating Response Times...\n";

  string cmd = "octave --no-window-system -q --eval \"response_time('" + prefix + "')\"";
  
  system(cmd.c_str());
}

///This function calls the Utilization script to produce "$prefix_Utilization.csv"
void analyzeUtilization(){
  cout << "Calculating Utilization...\n";

  string cmd = "octave --no-window-system -q --eval \"utilization('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the ResourceAllocationCost script to produce "$prefix_alloc_cost_us.csv"
void  analyzeResourceAllocationCost(){
  cout << "Calculating Resource Allocation Cost...\n";

  string cmd = "octave --no-window-system -q --eval \"resourceAllocationCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the analyzeSystemCost script to show "$prefix_System Cost"
void  analyzeSystemCost(){
  cout << "Calculating System Cost...\n";
  string cmd = "octave --no-window-system -q --eval \"systemCost('" + prefix + "')\"";

  system(cmd.c_str());
}

////This function calls the analyzeWorkerCost script to show "$prefix_Worker Cost"
void analyzeWorkerCost(){
  cout << "Calculating Worker Cost...\n";
  string cmd = "octave --no-window-system -q --eval \"workerCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function interprets all input parameters
void interpret(char *str) {
  //empty
}
