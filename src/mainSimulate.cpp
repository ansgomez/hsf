/*********************************
 *
 * Input parameter 1 .. N - names of HSF xml files to execute
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

///This function interprets all input parameters
void interpret(char *str);

///This functions executes, analyzes and shows the metrics of the input file
void simulate(string f);

/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {

  cout << "Starting HSF's simulate tool\n";

  ///If there are input parameters, interpret them
  if(argn > 1) {
    for(int i=1;i<=argn;i++) {
      interpret(new string(argv[i]));
    }
  }
  ///otherwise analyze all by default, 
  else {
    simulate("hsf");
  }

  cout << "\nDone." <<endl;

  return 0;
}

///This functions executes, analyzes and shows the metrics of the input file
void simulate(string f) {
  
}

///This function interprets all input parameters
void interpret(char *str) {
  
  //check if filename doesn't have an .xml extension, and if so add it
  if(str.find(".xml") == string::npos) {
    str = str+".xml";
  }

  cout << "Going to simulate: " << str << end;
}
