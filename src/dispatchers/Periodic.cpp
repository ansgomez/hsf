#include "dispatchers/Periodic.h"

#include "core/Simulation.h"
#include "core/Worker.h"
#include "results/Statistics.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

Periodic::Periodic(unsigned int id) : Dispatcher(id) {
  period =  TimeUtil::Millis(20);
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM DISPATCHER ****/  

void Periodic::dispatch() {
  struct timespec rem;

  while (Simulation::isSimulating()) {

    Statistics::addTrace(dispatcher, worker->getId(), task_arrival);

    if(worker != NULL) {
      cout << "Disp : " << id << " is registering new job\n";
      //worker->new_job();
      cout << "Disp : " << id << " is continuing execution\n";
    }
    else {
      cout << "Dispatcher error: worker is null!\n";
    }

    if(Simulation::isSimulating()) {
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

/*********** GETTER AND SETTER FUNCTIONS ***********/

///This function returns the period
struct timespec Periodic::getPeriod() {
  return period;
}

///This function sets the dispatcher's period
void Periodic::setPeriod(struct timespec p) {
  period = p;
}

