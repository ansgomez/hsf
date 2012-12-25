#include "pthread/Idle.h"

#include "core/Simulation.h"
#include "pthread/Priorities.h"
#include "util/Enumerations.h"

#include <iostream>

/********************* CONSTRUCTOR *********************/

///The constructor needs the simulation pointer
Idle::Idle(Simulation *s) : Thread(s,0) {
  sim = s;

  thread_type = idle;

  setPriority(Priorities::get_idle_pr());
}

/********************* MEMBER FUNCTIONS *********************/

///This function contains the flagged loop.
void Idle::run() {

#if _INFO == 1
  cout << "Starting Idle thread\n";
#endif

  while (Simulation::isSimulating() == 1);

#if _INFO == 1
  cout << "Exiting Idle thread\n";
#endif  
}

/********************* INHERITED FUNCTIONS *********************/

///This inherited function will only call run()
void Idle::wrapper() {
  run();
}
