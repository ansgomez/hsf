#include "pthread/Idle.h"

#include "core/Simulation.h"
#include "pthread/Priorities.h"
#include "util/Enumerations.h"

#include <iostream>

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///The constructor needs the simulation pointer
Idle::Idle() : Thread(0) {
  thread_type = idle;

  setPriority(Priorities::get_idle_pr());
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///This inherited function will only call run()
void Idle::wrapper() {
  run();
}

/*********** MEMBER FUNCTIONS ***********/

///This function contains the flagged loop.
void Idle::run() {

#if _INFO == 1
  cout << "Starting Idle thread\n";
#endif

  while (Simulation::isSimulating());

#if _INFO == 1
  cout << "Exiting Idle thread\n";
#endif  
}

