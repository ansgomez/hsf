#include "PeriodicJitter.h"

#include "Worker.h"
#include "Simulation.h"
#include "Trace.h"
#include "Operators.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/

PeriodicJitter::PeriodicJitter(Simulation *s, unsigned int id) : Dispatcher(s,id) {
  period =  Millis(20);

  srand(time(NULL));

  deltaPeriod = Millis(0);
}

/*********** INHERITED FUNCTIONS ***********/

void PeriodicJitter::dispatch() {
  struct timespec newPeriod, rem;
  double random;

  while (sim->isSimulating() ==  1) {

    sim->getTraces()->add_trace(dispatcher, worker->getId(), task_arrival);

    if(worker != NULL) {
      worker->new_job();
    }
    else {
      cout << "Dispatcher error: worker is null!\n";
    }

    if(sim->isSimulating() ==  1) {
      random = (1+(rand()%200))/100.0; //random in [1/100,2]
      newPeriod = period + jitter - random*jitter - deltaPeriod;
      //deltaPeriod keeps the timing to n*P+-jitter
      deltaPeriod = jitter - random*jitter;
      nanosleep(&newPeriod, &rem);
    }
  }

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
void PeriodicJitter::setPeriod(struct timespec p) {
  period = p;
}

///This function sets the jitter
void PeriodicJitter::setJitter(struct timespec j) {
  jitter = j;
}

///This function returns the period
struct timespec PeriodicJitter::getPeriod() {
  return period;
}


