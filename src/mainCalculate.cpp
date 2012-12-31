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
const char *metricsVector[] = {"execution_times","exec","response_times","resp","utilization","util","resource_allocation_cost","alloc", "system_cost","sys","worker_cost","worker", "all","exe","throughput"};
vector <string> metrics(metricsVector, (metricsVector)+sizeof(metricsVector)/sizeof(metricsVector[0]));

/*********** FUNCTIONS ***********/

///This function calls the execution_times script to produce "$prefix_execution_times.csv"
void calculateExecutionTimes();

///This function calls the Resource Allocation Cost script to produce "$prefix_alloc_cost_us.csv"
void  calculateResourceAllocationCost();

///This function calls the response_times script to produce "$prefix_response_times.csv"
void calculateResponseTimes();

///This function calls the SystemCost script to produce "$prefix_System Cost"
void  calculateSystemCost();

///This function calls the throughput script to produce "$prefix_Throughput"
void calculateThroughput();

///This function calls the Utilization script to produce "$prefix_Utilization.csv"
void calculateUtilization();

////This function calls the WorkerCost script to produce "$prefix_Worker Cost"
void calculateWorkerCost();

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


/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {

  string* aux;

#if _INFO==1
  cout << "Starting HSF's calculate tool\n";
#endif

  ///If there are input parameters, interpret them
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
  ///otherwise calculate all by default, 
  else {
    cout << "\n***   Calculating all metrics!\t***\n\n";
    calculateExecutionTimes();
    calculateResponseTimes();
    calculateThroughput();
    calculateUtilization();
    calculateResourceAllocationCost();
    calculateSystemCost();
    calculateWorkerCost();
  }

#if _INFO==1
  cout << "\nDone." <<endl;
#endif

  return 0;
}


///This function calls the execution_times script to produce "$prefix_execution_times.csv"
void calculateExecutionTimes() {
  cout << "  Calculating Execution Times...\n";

  string cmd = "octave --no-window-system -q --eval \"execution_time('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the ResourceAllocationCost script to produce "$prefix_alloc_cost_us.csv"
void  calculateResourceAllocationCost(){
  cout << "  Calculating Resource Allocation Cost...\n";

  string cmd = "octave --no-window-system -q --eval \"resourceAllocationCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the response_times script to produce "$prefix_response_times.csv"
void calculateResponseTimes() {
  cout << "  Calculating Response Times...\n";

  string cmd = "octave --no-window-system -q --eval \"response_time('" + prefix + "')\"";
  
  system(cmd.c_str());
}
///This function calls the SystemCost script to show "$prefix_System Cost"
void  calculateSystemCost(){
  cout << "  Calculating System Cost...\n";
  string cmd = "octave --no-window-system -q --eval \"systemCost('" + prefix + "')\"";

  system(cmd.c_str());
}

///This function calls the throughput script to produce "$prefix_Throughput"
void calculateThroughput(){
cout << "  Calculating Throughput...\n";
  string cmd = "octave --no-window-system -q --eval \"throughput('" + prefix + "')\"";

  system(cmd.c_str());

}
///This function calls the Utilization script to produce "$prefix_Utilization.csv"
void calculateUtilization(){
  cout << "  Calculating Utilization...\n";

  string cmd = "octave --no-window-system -q --eval \"utilization('" + prefix + "')\"";

  system(cmd.c_str());
}

////This function calls the WorkerCost script to show "$prefix_Worker Cost"
void calculateWorkerCost(){
  cout << "  Calculating Worker Cost...\n";
  string cmd = "octave --no-window-system -q --eval \"workerCost('" + prefix + "')\"";

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

///checks if the input string calls for throughput function
bool isThroughput(string str){
  if ((str.compare(metricsVector[14])==0))
      return true;
  else
      return false;
}

///This function process 
void process (){
  unsigned int i,j;
 
   for (i=0; i< inputPrefix.size(); i++){
     prefix = inputPrefix[i];
     for (j=0; j< inputMetric.size() ; j++){

       if( isExecution(inputMetric[j]) )
	 calculateExecutionTimes();
       if ( isResponseTimes(inputMetric[j]) )
	 calculateResponseTimes();
       if ( isUtilization(inputMetric[j]) )
	 calculateUtilization();
       if ( isResourceAllocationCost(inputMetric[j]) )
	 calculateResourceAllocationCost();
       if ( isSystemCost(inputMetric[j]) )
	 calculateSystemCost();
       if ( isWorkerCost(inputMetric[j]) )
         calculateWorkerCost();
       if ( isThroughput(inputMetric[j]) )
	 calculateThroughput();
       if ( isAll(inputMetric[j]) ){
	 cout << "***   Calculating all metrics!\t***\n\n";
	 calculateExecutionTimes();
	 calculateResponseTimes();
	 calculateThroughput();
	 calculateUtilization();
	 calculateResourceAllocationCost();
	 calculateSystemCost();
	 calculateWorkerCost();
       }
     }
   }
}


