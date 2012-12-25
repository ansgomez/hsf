#include <iostream>

#include "core/Simulation.h"

int main(int argc, char** argv) {
    
  Simulation *sim = new Simulation("hsf.xml", 1, "simulation");

  sim->simulate();

#if _INFO == 1
  cout << "Exiting main\n";
#endif

  return 0;
}
