#include "PeriodicJitterJitter.h"

#include "Worker.h"
#include "Simulation.h"
#include "Trace.h"
#include "Operators.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/********************* CONSTRUCTOR *********************/

PeriodicJitter::PeriodicJitter(Simulation *s, unsigned int id) : Dispatcher(s,id) {
  period =  Millis(20);

  relativeDeadline = period;
}

/********************* INHERITED FUNCTIONS *********************/

void PeriodicJitter::dispatch() {
  struct timespec rem;

  while (sim->isSimulating() ==  1) {

    sim->getTraces()->add_trace(dispatcher, worker->getId(), task_arrival);

    if(worker != NULL) {
      worker->new_job(relativeDeadline);
    }
    else {
      cout << "Dispatcher error: worker is null!\n";
    }

    if(sim->isSimulating() ==  1) {
      nanosleep(&period, &rem);
    }
  }

  //Free worker from blocking. This runs only when the simulation has ended
  if(worker != NULL) {
    worker->new_job(relativeDeadline);
  }
  else {
    cout << "Dispatcher::dispatch - Worker NULL problem\n";
  }

#if _INFO == 1
  cout << "Dispatcher " << id << " is exiting dispatch()\n";
#endif
}

///This function sets the dispatcher's period
void PeriodicJitter::setPeriod(struct timespec p) {
  period = p;

  relativeDeadline = period;
}

///This function sets the jitter
void PeriodicJitter::setJitter(struct timespec j) {
  jitter = j;
}

///This function returns the period
struct timespec PeriodicJitter::getPeriod() {
  return period;
}


