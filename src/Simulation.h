#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <vector>
#include <string>

using namespace std;

class Dispatcher;
class Parser;
class Priorities;
class Trace;
class Statistics;
class Scheduler;
class Idle;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Simulation {
 protected:

  ///This attribute indicates if there is currently a simulation
  int simulating;  

  ///The name of the simulation (will be used for traced output)
  string name;

  ///This object will hold all of the simulation's traces
  Trace *traces;

  ///Thread RuntimeStatistics
  Statistics *stats;

  ///This vector holds all of the relevant dispatchers
  vector<Dispatcher*> disp;

  ///This object is the root scheduler
  Scheduler *top_sched;

  ///This attribute holds how long the simulation will last
  struct timespec sim_time;

  ///This is the idle thread
  Idle *idle;

  ///Auxiliary variable to the SimulationFigure
  vector<unsigned int> worker_id;

  ///Auxiliary variables for the main thread
  cpu_set_t set;
  struct sched_param param;

 public:
  /************** CONSTRUCTOR *************/

  ///Constructor needs the path to xml and the cpu_set
  Simulation(string xml_path, int cpu, string name);

  /************** MEMBER FUNCTIONS *************/

  ///This function initializes all of the objects
  void initialize();
  void initialize_periodic_tdma();

  ///This function begins the simulation
  void simulate();

  ///This function tells if there is currently a simulation
  int isSimulating();

  ///This function sets the dispatchers to their 'active' priority.
  void activate_dispatchers();

  ///This function waits for all other thread to join
  void join_all();

  ///This function should be called by the Worker constructor to 'register' its id
  void add_worker_id(unsigned int _id);

  /************** GETTER FUNCTIONS *************/

  ///This function returns the name of the simulation
  string getName();

  ///This function returns the simulation's traces
  Trace* getTraces();

  ///This function returns a vector of the worker id's
  vector<unsigned int>* getWorker_id();

  ///This function returns the simulation time
  struct timespec getSim_time();

  //This function return the simulation's statistics
  Statistics* getStats();
};
#endif
