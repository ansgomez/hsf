#include "Statistics.h"

#include "Enumerations.h"
#include "RuntimeStatistic.h"

#include <time.h>
#include <fstream>
#include <iostream>

using namespace std;

///Contructor needs the simulation (for the name)
Statistics::Statistics(Simulation *s) {
  sim = s;
  stats.reserve(30);
}

///This function adds a trace to the vector
void Statistics::add_stat(enum _thread_type type, unsigned int id, struct timespec ts) {
#if _DEBUG==1
  cout << "Added statistic!\n";
#endif
  RuntimeStatistic *rs = new RuntimeStatistic(type, id, ts);
  stats.push_back(rs);
}

///This function saves to custom csv file
void Statistics::to_file() {
  ofstream file;
  RuntimeStatistic *aux;

#if _INFO == 0
cout << "Saving " << stats.size() << " stats to file...\n";
#endif

 sort(stats.begin(), stats.end(), compareId());

  file.open((sim->getName()+"_stats.csv").data());

  for(unsigned int c=0;c<stats.size();c++) {

    aux = stats[c];
    file << aux->toString() << "\n"; 
  }

  file.close();
}
