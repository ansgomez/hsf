//#include "defines.h"
#include <iostream>

#include "Simulation.h"

int main() {
    
  Simulation *sim = new Simulation("path", 1, "sim.csv");

  sim->simulate();

#if _INFO == 1
  cout << "Exiting main\n";
#endif

  return 0;
}
