#include "Aperiodic.h"

#include "Worker.h"
#include "Simulation.h"
#include "Trace.h"
#include "Operators.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Aperiodic::Aperiodic(Simulation *s, unsigned int id) : Dispatcher(s,id) {
  release_time = Millis(5); //default release time is 5 ms
}

///This function was a flagged loop that activates the Worker according to the task periodicity
void Aperiodic::dispatch() {
  struct timespec rem;

  nanosleep(&release_time, &rem);

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
  
  //wait until simulation is done to free worker
  do {
    struct timespec aux = sim->getSim_time() - release_time;
      nanosleep(&aux, &rem);
  } while(sim->isSimulating() == 1);
  
  //Free worker from blocking (doesn't affect workers while they have inactive priority -> only when the simulation has ended
  if(worker != NULL) {
    worker->new_job();
  }
}

void Aperiodic::setRelease(struct timespec r) {
  release_time = r;
}
struct timespec Aperiodic::getRelease() {
  return release_time;
}
