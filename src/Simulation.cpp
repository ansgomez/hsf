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

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Simulation::Simulation(string xml_path, int cpu, string nm) {
  //  struct sched_param param = {0};
  name = nm;

  sim_time =  Seconds(1);

  simulating = 0;

  //  cout << "Loading xml file...\n";

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

  //Reserve some memory for vectors
  disp.reserve(10);
  worker_id.reserve(10);

  initialize_periodic_tdma();
}

///This function initializes all of the objects
void Simulation::initialize() {
  traces = new Trace(this);
  stats = new Statistics(this);

  //Idle should be the first thread to be created
  idle = new Idle(this);

  TDMA *sched = new TDMA(this, 1);
  top_sched = (Scheduler*) sched;

  disp.push_back((Dispatcher*) new Aperiodic(this, 2));
  disp.push_back((Dispatcher*) new Periodic(this, 3));

  Worker *w;
  BusyWait  *t;
  struct timespec wcet;

  //add first worker
  wcet = Millis(5); //5 ms
  t = new BusyWait(this, disp[0], wcet);
  w = new Worker(this, top_sched, 4, busy_wait);
  w->setLoad(t);
  top_sched->add_load(w);
  disp[0]->setWorker(w);

  //add second worker
  wcet = Millis(9); //10 ms
  t = new BusyWait(this, disp[1], wcet);
  w = new Worker(this, top_sched, 5, busy_wait);
  w->setLoad(t);
  top_sched->add_load(w);
  disp[1]->setWorker(w);
  
  struct timespec ts;

  //add timeslot 1
  ts = Millis(10); //10 ms
  sched->add_slot(ts);

  //add timeslot 2
  ts = Millis(20); //20 ms
  sched->add_slot(ts);

  cout << "HSF has been initialized: Aperiodic TDMA\n";
}


///This function initializes all of the objects
void Simulation::initialize_periodic_tdma() {
  traces = new Trace(this);
  stats = new Statistics(this);

  //Idle should be the first thread to be created
  idle = new Idle(this);

  TDMA *sched = new TDMA(this, 1);
  top_sched = (Scheduler*) sched;

  disp.push_back((Dispatcher*) new Periodic(this, 2));
  disp.push_back((Dispatcher*) new Periodic(this, 3));

  Worker *w;
  BusyWait  *t;
  struct timespec wcet;
  wcet.tv_sec = 0;

  //add first worker
  wcet.tv_nsec = 5000000; //1ms
  t = new BusyWait(this, disp[0], wcet);
  w = new Worker(this, top_sched, 4, busy_wait);
  w->setLoad(t);
  top_sched->add_load(w);
  disp[0]->setWorker(w);

  //add second worker
  wcet.tv_nsec = 10000000; //2ms
  t = new BusyWait(this, disp[1], wcet);
  w = new Worker(this, top_sched, 5, busy_wait);
  w->setLoad(t);
  top_sched->add_load(w);
  disp[1]->setWorker(w);
  
  struct timespec ts;
  ts.tv_sec = 0;

  //add timeslot 1
  ts.tv_nsec = 10000000; //10ms
  sched->add_slot(ts);

  //add timeslot 2
  ts.tv_nsec = 20000000; //20ms
  sched->add_slot(ts);

  cout << "HSF has been initialized: Periodic TDMA\n";
}

///This function sets the dispatchers to their 'active' priority.
void Simulation::activate_dispatchers() {
  for (unsigned int c=0;c<disp.size();c++) {
    disp[c]->activate();
  }
}

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
  cout << "**Done**\n";
  simulating = 0;  
  top_sched->deactivate();

  //Join all other threads
  join_all();

  //Save statistics to file
  stats->to_file();

  //Save traces to file
  traces->to_file();
  traces->to_figure();

  cout << "All results have saved!\n";
}

///This function waits for all other thread to join
void Simulation::join_all() {
  //Wait for all dispatchers
  for(unsigned int c=0;c<disp.size();c++) {
    pthread_join(*(disp[c]->getPthread()), NULL);
  }
   
  //Wait for the idle thread (all other threads should be deactivated
  pthread_join(*(idle->getPthread()), NULL);

  top_sched->join_all();

  //TODO: call functions that post to all semaphores
  pthread_join(*(top_sched->getPthread()), NULL);
}

///This function should be called by the Worker constructor to 'register' its id
void Simulation::add_worker_id(unsigned int _id) {
  worker_id.push_back(_id);
}

int Simulation::isSimulating() {
  return simulating;
}

string Simulation::getName() {
  return name;
}

Trace* Simulation::getTraces() {
  return traces;
}

Statistics* Simulation::getStats() {
  return stats;
}

///This function returns a vector of the worker id's
vector<unsigned int>* Simulation::getWorker_id() {
  return &worker_id;
}

struct timespec Simulation::getSim_time() {
  return sim_time;
}
