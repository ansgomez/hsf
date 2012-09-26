#include "Periodic.h"
//#include "defines.h"

#include "Worker.h"
#include "Simulation.h"
#include "Trace.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Periodic::Periodic(Simulation *s, unsigned int id) : Dispatcher(s,id) {
  period.tv_sec = 0;
  period.tv_nsec = 10000000; //default period is 10 ms
}

void Periodic::dispatch() {
  struct timespec rem;

  while (sim->isSimulating() ==  1) {

#if _INFO == 1
    cout << "Disp " << id << ": new job!\n";
#endif  

    sim->getTraces()->add_trace(dispatcher, id, task_arrival);

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

  //Free worker from blocking (doesn't affect workers while they have inactive priority -> only when the simulation has ended
  if(worker != NULL) {
    worker->new_job();
  }
}
