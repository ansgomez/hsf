#include "Simulation.h"
//#include "defines.h"

#include "Dispatcher.h"
#include "Scheduler.h"
#include "Worker.h"
#include "Aperiodic.h"
#include "Periodic.h"
#include "Parser.h"
#include "Priorities.h"
#include "Trace.h"
#include "Statistics.h"
#include "TDMA.h"
#include "TimeUtil.h"
#include "BusyWait.h"
#include "Task.h"
#include "Idle.h"
#include "Operators.h"

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */
/********************* CONSTRUCTOR *********************/
  ///Constructor needs the path to xml and the cpu_set
Simulation::Simulation(string _xml_path, int cpu, string nm) {
  name = nm;
  sim_time = Seconds(1); //Millis(100); //
  simulating = 0;
  xml_path = _xml_path;

  //Set main process parameters
  param.sched_priority = Priorities::get_main_pr(); //highest priority
  if ((sched_setscheduler(0, SCHED_FIFO, &param) != 0)) {
    cout << "Run with root\n";
    pthread_exit(0);
  }

  //TODO: other options, and default
  //  if (cpu==1)
  {
    //Set CPU affinity
    CPU_ZERO (&set);
    CPU_CLR (0, &set);
    CPU_CLR (1, &set);
    CPU_CLR (2, &set);
    CPU_SET (3, &set);
  }

  if (pthread_setaffinity_np(pthread_self(), sizeof (cpu_set_t), &set) <0) {
    perror("Error setting CPU affinity\n");
  }

  initialize();
}

/********************* MEMBER FUNCTIONS *********************/

///This function initializes all of the objects
void Simulation::initialize() {
  traces = new Trace(this);
  stats = new Statistics(this);

  //Idle should be the first thread to be created
  idle = new Idle(this);

  cout << "Loading xml file...";

  Parser *parser = new Parser(this);

  parser->parseFile(xml_path);

  free(parser);

  //Reserve some memory for vectors
  disp.reserve(10);
  worker_id.reserve(10);
}

///This function sets the dispatchers to their 'active' priority.
void Simulation::activate_dispatchers() {
  for (unsigned int c=0;c<disp.size();c++) {
    disp[c]->activate();
  }
}

///This function begins the simulation
void Simulation::simulate() {
  struct timespec rem;
  cout << "**Simulating**\n" ;

  //Set simulation variables
  simulating = 1;  
  TimeUtil::setOffset();

  //Activate threads
  activate_dispatchers();
  top_sched->activate();

  //Sleep for the duration of the simulation
  nanosleep(&sim_time, &rem);

  //Deactivate threads
  simulating = 0;  
  cout << "**Done**\n";
  //top_sched->deactivate();

#if _INFO==1
  cout << "Waiting for threads to exit...\n";
#endif

  //Join all other threads
  join_all();

  cout << "Saving results...\n";
  //Save statistics to file
  stats->to_file();
  //Save traces to file
  traces->to_file();
  //Save figure to file

  //For experimentation, figures are unnecessary
  //traces->to_figure();
  cout << "All results have been saved!\n";
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

///This function should be called by the Worker constructor to 'register' its id
void Simulation::add_worker_id(unsigned int _id) {
#if _INFO ==1
  cout << "Adding Worker ID: " << _id << endl;
#endif 

  worker_id.push_back(_id);

}

///This function tells if there is currently a simulation
int Simulation::isSimulating() {
  return simulating;
}

///This function adds dispatchers to the simulation object
void Simulation::addDispatcher(Dispatcher *d) {
  disp.push_back(d);
}

/********************* GETTER AND SETTER FUNCTIONS *********************/

///This function returns the name of the simulation
string Simulation::getName() {
  return name;
}

///This function returns the simulation's traces
Trace* Simulation::getTraces() {
  return traces;
}

///This function return the simulation's statistics
Statistics* Simulation::getStats() {
  return stats;
}

///This function returns a vector of the worker id's
vector<unsigned int> Simulation::getWorker_id() {
  return worker_id;
}

///This function returns the simulation time
struct timespec Simulation::getSim_time() {
  return sim_time;
}

///This function sets the name of the simulation
void Simulation::setName(string s) {
  name = s;
}

///This function sets the duration of the simulation
void Simulation::setDuration(struct timespec d) {
  sim_time = d;
}

///This function sets the top schedulerxs
void Simulation::setTopScheduler(Scheduler *sched) {
  top_sched = sched;
}
