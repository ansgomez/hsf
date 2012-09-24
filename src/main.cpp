#include <iostream>

#include "Simulation.h"

#define _INFO   1
#define _DEBUG  1

int main() {
    
  Simulation *sim = new Simulation("path", 1, "sim.csv");

  sim->simulate();

#if _INFO == 1
  cout << "Exiting main\n";
#endif

  return 0;
}
