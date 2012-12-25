#include "dispatchers/Aperiodic.h"

#include "core/Worker.h"
#include "core/Simulation.h"
#include "results/Statistics.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/********************* CONSTRUCTOR *********************/

///Constructor needs a pointer to simulation and id
Aperiodic::Aperiodic(Simulation *s, unsigned int id) : Dispatcher(s,id) {
  release_time = TimeUtil::Millis(5); //default release time is 5 ms
}

/********************* INHERITED FUNCTIONS *********************/

///This function was a flagged loop that activates the Worker according to the task periodicity
void Aperiodic::dispatch() {
  struct timespec rem;

  nanosleep(&release_time, &rem);

#if _INFO == 1
  cout << "+Worker " << worker->getId() << " has new job!\n";
#endif  

  Statistics::addTrace(dispatcher, worker->getId(), task_arrival);
  
  if(worker != NULL) {
    worker->new_job();
  }
  else {
    cout << "Dispatcher error: worker is null!\n";
  }
  
  //wait until simulation is done to free worker
  do {
    struct timespec aux = Simulation::getSimTime() - release_time;
      nanosleep(&aux, &rem);
  } while(Simulation::isSimulating() == 1);
  
  //Free worker from blocking (doesn't affect workers while they have inactive priority -> only when the simulation has ended
  if(worker != NULL) {
    worker->new_job();
  }
}

/********************* GETTER AND SETTER FUNCTIONS *********************/

///This function sets the release time for the aperiodic dispatcher
void Aperiodic::setRelease(struct timespec r) {
  release_time = r;
}

///This function returns the release time
struct timespec Aperiodic::getRelease() {
  return release_time;
}
