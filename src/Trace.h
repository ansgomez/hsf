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
  vector<JobTrace> traces;
  Simulation * sim;
  int trace_full;
  sem_t trace_sem;

public:
  ///Contructor
  Trace(Simulation *s);

  ///This function adds a trace to the vector
  void add_trace(const enum _thread_type & type, const unsigned int & t_id, const enum _task_action & act);

  ///This function saves to custom csv file
  void to_file();

  ///This function generates a figure from traces
  void to_figure();
};


#endif
