#ifndef _SIMULATION_H
#define _SIMULATION_H

#include <string>
#include <time.h>
#include <vector>

class Dispatcher;
class Idle;
class Parser;
class Priorities;
class Scheduler;
class Statistics;
class Thread;
class Trace;

using namespace std;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Simulation {

 protected:

  /*********** VARIABLES ***********/

  /**** INPUT VARIABLES ****/

  ///The name of the simulation (will be used for traced output)
  string name;

  ///This attribute holds how long the simulation will last
  static struct timespec simTime;

  ///This variable stores the path to the XML input file
  string xml_path;

  /**** HSF VARIABLES */

  ///This object is the root scheduler
  Scheduler* top_sched;

  ///This vector holds all of the simulation's dispatchers
  vector<Dispatcher*> disp;

  ///This is the idle thread
  Idle* idle;

  /**** STATIC VARIABLES ****/

  ///This attribute indicate if the simulation is initialized
  static bool initialized;

  ///This attribute indicates if there is currently a simulation
  static bool simulating;  

  /**** AUXILIARY ****/

  ///This vector holds a pointer to all Thread's
  vector<Thread*> threads;

  ///Auxiliary variable for the cpu affinity
  cpu_set_t set;

  ///Auxiliary variable for the main thread's parameters
  struct sched_param param;

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs the path to xml and the cpu_set
  Simulation(string xml_path, int cpu);

  /*********** MEMBER FUNCTIONS ***********/

  ///This function sets the dispatchers to their 'active' priority.
  void activate_dispatchers();

  ///This function adds dispatchers to the simulation object
  void addDispatcher(Dispatcher *d);

  ///This function adds thread pointers to the simulation object
  void addThread(Thread* t);

  ///This function initializes all of the objects
  void initialize();

  ///This function waits for all other thread to join
  void join_all();

  ///This function performs the simulation
  void simulate();

  /*********** STATIC FUNCTIONS ***********/

  ///This function returns the simulation time
  static struct timespec getSimTime();

  ///This function tells whether the simulation is initialized (used to synchronize threads during X11_init()
  static bool isInitialized();

  ///This function returns true when the simulation is active
  static bool isSimulating();

  /*********** GETTER AND SETTER FUNCTIONS ***********/

  ///This function returns the name of the simulation
  string getName();
  ///This function sets the duration of the simulation
  void setDuration(struct timespec d);
  ///This function sets the name of the simulation
  void setName(string s);
  ///This function sets the top scheduler
  void setTopScheduler(Scheduler* sched);
};
#endif
