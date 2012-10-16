#include "Periodic.h"

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

Periodic::Periodic(Simulation *s, unsigned int id) : Dispatcher(s,id) {
  period =  Millis(20);
}

/********************* INHERITED FUNCTIONS *********************/

void Periodic::dispatch() {
  struct timespec rem;

  while (sim->isSimulating() ==  1) {

#if _INFO == 1
    cout << "+Worker " << worker->getId() << " has new job!\n";
#endif  

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

  //Free worker from blocking (doesn't affect workers while they have
  //inactive priority -> only when the simulation has ended)
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
