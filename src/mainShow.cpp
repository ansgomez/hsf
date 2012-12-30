/*********************************
 *
 * Input parameter 1    - file prefix
 * Input parameter 2..N - metric to compute
 *
 * Metrics include:
 *
 * execution_times              (exec/exe)
 * response_times               (resp)
 * utilization                  (util)
 * resource_allocation_cost     (alloc)
 * system_implementation_cost   (sys)
 * worker_cost                  (worker)
 * throughput                   (throughput)
 * -----------------------------------
 * All of the above             (all)
 *
 *********************************/

#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

/*********** VARIABLES ***********/

///This variable holds the prefix for the input files (_traces, _runtimes, _missedDeadlines).
string prefix="simulation";
vector <string> inputMetric, inputPrefix;
const char *metricsVector[] = {"execution_times","exec","response_times","resp","utilization","util","resource_allocation_cost","alloc", "system_cost","sys","worker_cost","worker", "all","exe", "throughput"};
vector <string> metrics(metricsVector, (metricsVector)+sizeof(metricsVector)/sizeof(metricsVector[0]));

/*********** FUNCTIONS ***********/

///This function interprets all input parameters
void interpret(string str);

///checks if the input string calls for all functions
bool isAll(string str);

///checks if the input string calls for execution function
bool isExecution(string str);

////checks if the input string calls for resource allocation function
bool isResourceAllocationCost(string str);

////checks if the input string calls for ResponseTimes function
bool isResponseTimes(string str);

///checks if the input string calls for system cost function
bool isSystemCost(string str);

///checks if the input string calls for throughput function
bool isThroughput(string str);

///checks if the input string calls for utilization function
bool isUtilization(string str);

///checks if the input string calls for worker cost functio
bool isWorkerCost(string str);

///This function process all the imput metrices with all prefixes
void process();

///This function calls the show_execution_times script to produce "$prefix_execution_times.csv"
void showExecutionTimes();

///This function calls the Resource Allocation Cost script to produce "$prefix_alloc_cost_us.csv"
void showResourceAllocationCost();

///This function calls the show_response_times script to produce "$prefix_response_times.csv"
void showResponseTimes();

///This function calls the showSystemCost script to show "$prefix_System Cost"
void showSystemCost();

///This function calls the showThroughput script to show "$prefix_Throughput"
void showThroughput();

///This function calls the show_Utilization script to produce "$prefix_Utilization.csv"
void showUtilization();

///This function calls the showWorkerCost script to show "$prefix_Worker Cost"
void showWorkerCost();


/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {

  string* aux;
#if _INFO==1
  cout << "Starting HSF's show tool\n";
#endif

  ///If there are input pArameters, interpret them
  if(argn > 1) {
    for(int i=1;i<=argn;i++) {
      if(argv[i] == NULL)
	continue;

      aux = new string(argv[i]);
      interpret(*aux);
    }

    if(inputMetric.size() == 0)
      inputMetric.push_back("all");
    if(inputPrefix.size() == 0)
      inputPrefix.push_back("simulation");
   
    cout << endl;

    process ();
  }   
  ///otherwise analyze all by default, 
  else {
    cout << "\n***   Showing all metrics!\t***\n\n";
    showExecutionTimes();
    showResponseTimes();
    showThroughput();
    showUtilization();
    showResourceAllocationCost();
    showSystemCost();
    showWorkerCost();
  }

#if _INFO==1
  cout << "\nDone." <<endl;
#endif

  return 0;
}


///This function calls the show_execution_times script to show "$prefix_execution_times"
void showExecutionTimes() {
  string cmd = "octave --no-window-system -q --eval \"show_execution_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showResourceAllocationCost script to show "$prefix_Resource Allocation Cost"
void  showResourceAllocationCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_resourceAllocationCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the show_response_times sCript to show "$prefix_response_times"
void showResponseTimes() {
  string cmd = "octave --no-window-system -q --eval \"show_response_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showSystemCost script to show "$prefix_System Cost"
void  showSystemCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_systemCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showThroughput script to show "$prefix_Throughput"
void showThroughput(){

  string cmd = "octave --no-window-system -q --eval \"show_throughput('" + prefix + "')\"";

  system(cmd.c_str());

}

///This function calls the show_utilization script to show "$prefix_utilization"
void showUtilization() {
  string cmd = "octave --no-window-system -q --eval \"show_utilization('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the showWorkerCost script to show "$prefix_Worker Cost"
void  showWorkerCost(){
 
  string cmd = "octave --no-window-system -q --eval \"show_workerCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function interprets all input parameters
void interpret(string str) {

  transform(str.begin(),str.end(),str.begin(),::tolower);
  if (find (metrics.begin(), metrics.end(), str)!=metrics.end())
    inputMetric.push_back(str);
  else
    inputPrefix.push_back(str);
}

///This function process 
void process (){
  unsigned int i,j;
 
   for (i=0; i< inputPrefix.size(); i++){
     prefix = inputPrefix[i];
     for (j=0; j< inputMetric.size() ; j++){

       if( isExecution(inputMetric[j]) )
	 showExecutionTimes();
       if ( isResponseTimes(inputMetric[j]) )
	 showResponseTimes();
       if ( isUtilization(inputMetric[j]) )
	 showUtilization();
       if ( isResourceAllocationCost(inputMetric[j]) )
	 showResourceAllocationCost();
       if ( isSystemCost(inputMetric[j]) )
         showSystemCost();
       if ( isWorkerCost(inputMetric[j]) )
	 showWorkerCost();
       if ( isThroughput(inputMetric[j]) )
	 showThroughput();
       if ( isAll(inputMetric[j]) ){
	 cout << "***   Showing all metrics!\t***\n\n";
	 showExecutionTimes();
	 showResponseTimes();
	 showThroughput();
	 showUtilization();
	 showResourceAllocationCost();
	 showSystemCost();
	 showWorkerCost();
	 
       }
     }
   }
}

///checks if the input string calls for execution function
bool isExecution(string str){
  if ((str.compare(metricsVector[0])==0)||(str.compare(metricsVector[1])==0)||(str.compare(metricsVector[13])==0))
      return true;
  else
      return false;
}

///checks if the input string calls for ResponseTimes function
bool isResponseTimes(string str){
  if ((str.compare(metricsVector[2])==0)||(str.compare(metricsVector[3])==0))
      return true;
  else
      return false;
}

///checks if the input string calls for utilization function
bool isUtilization(string str){
  if ((str.compare(metricsVector[4])==0)||(str.compare(metricsVector[5])==0))
      return true;
  else
      return false;
}

///checks if the input string calls for resource allocation function
bool isResourceAllocationCost(string str){
  if ((str.compare(metricsVector[6])==0)||(str.compare(metricsVector[7])==0))
      return true;
  else
      return false;
}

///checks if the input string calls for system cost function
bool isSystemCost(string str){
  if ((str.compare(metricsVector[8])==0)||(str.compare(metricsVector[9])==0))
      return true;
  else
      return false;
}

///checks if the input string calls for worker cost function
bool isWorkerCost(string str){
  if ((str.compare(metricsVector[10])==0)||(str.compare(metricsVector[11])==0))
      return true;
  else
      return false;
}

///checks if the input string calls for all functions
bool isAll(string str){
  if ((str.compare(metricsVector[12])==0)||(str.compare("all")==0))
      return true;
  else
      return false;
}

///checks if the input string calls for Throughput function
bool isThroughput(string str){
  if ((str.compare(metricsVector[14])==0))
      return true;
  else
      return false;
}
