#include "Trace.h"

#include "Enumerations.h"
#include "Simulation.h"
#include "TimeUtil.h"

#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;

#define MAX_TRACES 200

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

//Constructor requires a pointer to the simulation that the traces belong to
Trace::Trace(Simulation *s) {
  sim = s;

  trace_full = 0;
  
  traces.reserve(MAX_TRACES);//preallocate memory

  //Initialize semaphore to 1, so it acts like a mutex
  sem_init(&trace_sem, 0, 1);
}

///This function adds a trace to the vector
void Trace::add_trace(const enum _thread_type & type, const unsigned int & t_id, const enum _task_action & act) {

#if _DEBUG == 1
  cout << "Adding a new trace\n";
#endif

  if(sim->isSimulating() == 1) {
    if(traces.size() < MAX_TRACES) {
      JobTrace jt;
      struct timespec aux;

      //clock_gettime(CLOCK_REALTIME, &aux);
      aux = TimeUtil::getTime(relative);
      jt.setTrace(aux, type, t_id, act);

      sem_wait(&trace_sem);
      traces.push_back(jt);
      sem_post(&trace_sem);
    }
    else { //Trace array is full
      trace_full=1;
    }
  }
}

///This function saves to custom csv file
void Trace::to_file() {
  ofstream file;
  JobTrace *aux;

#if _INFO == 1
  cout << "Saving to file...\n";
#endif

  file.open(sim->getName().data());

  for(unsigned int c=0;c<=traces.size();c++) {

    aux = &traces[c];
    
    file << aux->toString() << "\n"; 
  }

  file.close();
}

///This function generates a figure from traces
void Trace::to_figure() {
  //TODO
}
