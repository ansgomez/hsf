#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <vector>
#include <string>

using namespace std;

class Dispatcher;
class Parser;
class Priorities;
class Trace;
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

  ///This vector holds all of the relevant dispatchers
  vector<Dispatcher*> disp;

  ///This object is the root scheduler
  Scheduler *top_sched;

  ///This attribute holds how long the simulation will last
  struct timespec sim_time;

  ///This is the idle thread
  Idle *idle;

  cpu_set_t set;
  struct sched_param param;

 public:
  /************** CONSTRUCTOR *************/

  ///Constructor needs the path to xml and the cpu_set
  Simulation(string xml_path, int cpu, string name);

  /************** MEMBER FUNCTIONS *************/

  ///This function initializes all of the objects
  void initialize();

  ///This function begins the simulation
  void simulate();

  ///This function tells if there is currently a simulation
  int isSimulating();

  ///This function sets the dispatchers to their 'active' priority.
  void activate_dispatchers();

  ///This function waits for all other thread to join
  void join_all();

  /************** GETTER FUNCTIONS *************/

  ///This function returns the name of the simulation
  string getName();

  ///This function returns the simulation's traces
  Trace* getTraces();

};
#endif
