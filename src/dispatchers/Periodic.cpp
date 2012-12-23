#include "dispatchers/Periodic.h"

#include "core/Worker.h"
#include "core/Simulation.h"
#include "results/Trace.h"
#include "util/Operators.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/********************* CONSTRUCTOR *********************/

Periodic::Periodic(Simulation *s, unsigned int id) : Dispatcher(s,id) {
  period =  Millis(20);
}

/********************* INHERITED FUNCTIONS *********************/

void Periodic::dispatch() {
  struct timespec rem;

  while (sim->isSimulating() ==  1) {

    sim->getTraces()->add_trace(dispatcher, worker->getId(), task_arrival);

    if(worker != NULL) {
      worker->new_job();
    }
    else {
      cout << "Dispatcher error: worker is null!\n";
    }

    if(sim->isSimulating() ==  1) {
      nanosleep(&period, &rem);
    }
  }


  //TODO: Verify is the code below can be erased -> worker::join now takes care of unblocking the worker

  //Free worker from blocking. This runs only when the simulation has ended
  if(worker != NULL) {
    worker->new_job();
  }
  else {
    cout << "Dispatcher::dispatch - Worker NULL problem\n";
  }

#if _INFO == 1
  cout << "Dispatcher " << id << " is exiting dispatch()\n";
#endif
}

///This function sets the dispatcher's period
void Periodic::setPeriod(struct timespec p) {
  period = p;
}

///This function returns the period
struct timespec Periodic::getPeriod() {
  return period;
}
