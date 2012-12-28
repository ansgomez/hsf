#include <iostream>

#include "core/Simulation.h"

#include <stdlib.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char** argv) {

  Simulation* sim;
  string* file;

  //If XML file names are received as input
  if(argc > 1) {

    //Iterate through all input names
    for(int i=1;i<=argc;i++) {
      struct stat buf;

      file = new string(argv[i]);

      if(file->find(".xml") == string::npos) {
	*file = *file + ".xml";
      }

      //if file exists, simulate it!
      if (stat(file->c_str(), &buf) != -1) {
	sim = new Simulation(*file, 1, "simulation");
	sim->simulate();
	cout << "\nFinished simulation #" << i << " ...\n\n";
	free(sim);
      }

      free(file);
    }
  }
  //Else look for default hsf.xml
  else {
    sim = new Simulation("hsf.xml", 1, "simulation");
    sim->simulate();
    free(sim);
  }

#if _INFO == 1
  cout << "Exiting main\n";
#endif

  return 0;
}
