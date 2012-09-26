#include "Idle.h"

#include "Simulation.h"

#include <iostream>

///The constructor needs the simulation pointer
Idle::Idle(Simulation *s) : Thread(0) {
  sim = s;
}

///This function contains the flagged loop.
void Idle::run() {
  while (sim->isSimulating() == 1);

#if _INFO == 0
  cout << "Exiting Idle thread\n";
#endif  
}

///This inherited function will only call run()
void Idle::wrapper() {
  run();
}
