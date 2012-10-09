#ifndef _STATISTICS_H
#define _STATISTICS_H

#include "Simulation.h"
#include "RuntimeStatistic.h"
#include "Enumerations.h"

#include <algorithm>
#include <vector>
#include <time.h>

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Statistics {
  /*********** VARIABLES ***********/
 private:
  ///Pointer to the simulation
  Simulation * sim;
  ///This vector holds all of the simulation's runtime statistics
  vector<RuntimeStatistic*> stats;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Contructor needs the simulation (for the name)
  Statistics(Simulation *s);

  /*********** MEMBER FUNCTIONS ***********/
  ///This function adds a trace to the vector
  void add_stat(enum _thread_type type, unsigned int t_id, struct timespec ts);

  ///This function helps sort the vector by ID
  struct compareId {
    bool operator()(RuntimeStatistic *a, RuntimeStatistic *b) {
      if (a->getId() < b->getId() ) {
	return true;
      }
      else {
	return false;
      }
    }
  };

  ///This function saves to custom csv file
  void to_file();
};
  
#endif
