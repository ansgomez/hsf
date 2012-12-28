#include "core/Simulation.h"
//#include "defines.h"

#include "core/Dispatcher.h"
#include "core/Scheduler.h"
#include "core/Task.h"
#include "core/Worker.h"
#include "dispatchers/Aperiodic.h"
#include "dispatchers/Periodic.h"
#include "pthread/Idle.h"
#include "pthread/Priorities.h"
#include "results/Statistics.h"
#include "results/Trace.h"
#include "util/Operators.h"
#include "util/Parser.h"
#include "util/TimeUtil.h"

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <pthread.h>

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** STATIC VARIABLES ***********/

///This attribute holds how long the simulation will last
struct timespec Simulation::simTime;
///This attribute indicate if the simulation is initialized
bool Simulation::initialized=false;
///This attribute indicates if there is currently a simulation
bool Simulation::simulating=false;  

/*********** CONSTRUCTOR ***********/
  ///Constructor needs the path to xml and the cpu_set
Simulation::Simulation(string _xml_path, int cpu, string nm) {
  name = nm;
  simTime = TimeUtil::Seconds(1); 
  initialized = false;
  simulating = false;
  xml_path = _xml_path;

  //Set main process parameters
  param.sched_priority = Priorities::get_main_pr(); //highest priority
  if ((sched_setscheduler(0, SCHED_FIFO, &param) != 0)) {
    cout << "Run with root\n";
    pthread_exit(0);
  }

  //Check if file exist...
  struct stat buf;
  if (stat(xml_path.c_str(), &buf) == -1) {
    cout << "\nSimulation::Simulation() error! File '" << xml_path << "' not found...\n";
    xml_path = "error";
    return;
  }

  int n_cpus = sysconf( _SC_NPROCESSORS_ONLN );
  //TODO: other options
  //  if (cpu==1)
  {
    //Set CPU affinity
     CPU_ZERO (&set);

     for(int i=0;i<n_cpus;i++) {
       //Set only the last CPU
       if(i==n_cpus-1) {
	 CPU_SET (i, &set);
       }
       //By default, clear CPU from the set
       else {
	 CPU_CLR (i, &set);
       }
     }
  }

  if (pthread_setaffinity_np(pthread_self(), sizeof (cpu_set_t), &set) <0) {
    perror("Error setting CPU affinity\n");
  }

  initialize();
}

/*********** MEMBER FUNCTIONS ***********/

///This function sets the dispatchers to their 'active' priority.
void Simulation::activate_dispatchers() {
  for (unsigned int c=0;c<disp.size();c++) {
    disp[c]->activate();
  }
}

///This function should be called by the Worker constructor to 'register' its id
void Simulation::add_worker_id(unsigned int _id) {
#if _INFO ==1
  cout << "Adding Worker ID: " << _id << endl;
#endif 

  worker_id.push_back(_id);

}

///This function initializes all of the objects
void Simulation::initialize() {
  //Reserve some memory for vectors
  disp.reserve(50);
  worker_id.reserve(50);
  Statistics::initialize();

  //Idle should be the first thread to be created
  idle = new Idle();

  Parser *parser = new Parser(this);

  parser->parseFile(xml_path);

  free(parser);
}

///This function waits for all other thread to join
void Simulation::join_all() {
  //Wait for all dispatchers
#if _DEBUG==1
  cout << "Waiting for dispatchers...\n";
#endif

  for(unsigned int c=0;c<disp.size();c++) {
    if(disp[c] != NULL) {
      disp[c]->join();
    }
  }

#if _DEBUG==1
  cout << "Waiting for idle...\n";
#endif
   
  if(idle != NULL) {
    idle->join();
  }

  //Wait for top_sched
#if _DEBUG==1
  cout << "Waiting for top_sched...\n";
#endif

  if(top_sched != NULL) {
    top_sched->join();
  }

#if _INFO==1
  cout << "Joined all!\n";
#endif
}


///This function begins the simulation
void Simulation::simulate() {

  if(xml_path.compare("error") == 0) {
    cout << "\nSimulation::simulate() error! Simulation was not loaded correctly!\n";
    return;
  }

  struct timespec rem;
  cout << "***   Simulating\t\t***\n" ;

  //Set simulation variables
  Statistics::enable();
  initialized=true;
  simulating=true;
  TimeUtil::setOffset();

  //Activate threads
  activate_dispatchers();
  top_sched->activate();

  //Sleep for the duration of the simulation
  nanosleep(&simTime, &rem);

  cout << "***   Done\t\t\t***\n";
  //Deactivate threads
  simulating=false;
  Statistics::disable();

  //Join all other threads
  join_all();

#if _INFO==1
  cout << "Saving results...\n";
#endif

  //Save results to file
  Statistics::toFile(name);

  cout << "***   Results Saved!\t\t***\n\n";
}

/*********** STATIC FUNCTIONS ***********/

///This function returns the simulation time
struct timespec Simulation::getSimTime() {
  return simTime;
}

///This function tells if the simulation is initialized
bool Simulation::isInitialized() {
  return initialized;
}

///This function tells if there is currently a simulation
bool Simulation::isSimulating() {
  return simulating;
}

/*********** GETTER AND SETTER FUNCTIONS ***********/

///This function adds dispatchers to the simulation object
void Simulation::addDispatcher(Dispatcher *d) {
  disp.push_back(d);
}

///This function returns the name of the simulation
string Simulation::getName() {
  return name;
}

///This function returns a vector of the worker id's
vector<unsigned int> Simulation::getWorker_id() {
  return worker_id;
}

///This function sets the duration of the simulation
void Simulation::setDuration(struct timespec d) {
  simTime = d;
}

///This function sets the name of the simulation
void Simulation::setName(string s) {
  name = s;
}

///This function sets the top schedulerxs
void Simulation::setTopScheduler(Scheduler *sched) {
  top_sched = sched;
}
