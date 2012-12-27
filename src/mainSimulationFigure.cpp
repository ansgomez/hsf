#include "../lib/mathgl/SimulationFigure.h"

#include <iostream>

int main(int argn, char **argv) {
  cout << "Starting SimFig\n";

  SimulationFigure *sf = new SimulationFigure("simulation");

  if(argn > 1) {
    sf->importCSV(argv[1]);
  }
  else {
    sf->importCSV("simulation_traces.csv");
  }

  sf->generateFigure();

  cout << "imported " <<endl;

  return 0;
}
