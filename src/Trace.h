#ifndef _TRACE_H
#define _TRACE_H

#include "Enumerations.h"
#include "JobTrace.h"

#include <vector>
#include <semaphore.h>

using namespace std;

class JobTrace;
class Simulation;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Trace {
private:
  /*********** VARIABLES ***********/
  ///Pointer to the simulation
  Simulation * sim;

  ///This vector will hold all of the simulation's traces
  vector<JobTrace> traces;

  ///Auxiliary variable to indicate if the trace vector was filled
  int trace_full;

  ///Semaphore to protect writing to the trace vector
  sem_t trace_sem;

  /*********** CONSTRUCTOR ***********/
public:
  ///Contructor
  Trace(Simulation *s);

  /*********** MEMBER FUNCTIONS ***********/
  ///This function adds a trace to the vector
  void add_trace(enum _thread_type type, unsigned int t_id, enum _task_action act);

  ///This function saves to custom csv file
  void to_file();

  ///This function generates a figure from traces
  void to_figure();
};


#endif
