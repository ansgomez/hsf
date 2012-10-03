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

Simulation::Simulation(string _xml_path, int cpu, string nm) {
  //  struct sched_param param = {0};
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
  //itialize_hierarchical_periodic_tdma();
}

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

///This function initializes all of the objects with APERIODIC TDMA EXAMPLE
void Simulation::initialize_aperiodic_tdma() {
  traces = new Trace(this);
  stats = new Statistics(this);
  worker_id.clear();

  //Idle should be the first thread to be created
  idle = new Idle(this);

  TDMA *sched = new TDMA(this, 1, 0); //id, level
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

///This function initializes all of the objects with APERIODIC TDMA EXAMPLE
void Simulation::initialize_periodic_tdma() {
  traces = new Trace(this);
  stats = new Statistics(this);

  //Idle should be the first thread to be created
  idle = new Idle(this);

  TDMA *sched = new TDMA(this, 1, 0); //id, level
  top_sched = (Scheduler*) sched;

  disp.push_back((Dispatcher*) new Periodic(this, 2));
  disp.push_back((Dispatcher*) new Periodic(this, 3));

  Worker *w;
  BusyWait  *t;
  struct timespec wcet;

  //add first worker
  wcet = Millis(5); //5ms
  t = new BusyWait(this, disp[0], wcet);
  w = new Worker(this, top_sched, 4, busy_wait);
  w->setLoad(t);
  top_sched->add_load(w);
  disp[0]->setWorker(w);

  //add second worker
  wcet = Millis(10); //2ms
  t = new BusyWait(this, disp[1], wcet);
  w = new Worker(this, top_sched, 5, busy_wait);
  w->setLoad(t);
  top_sched->add_load(w);
  disp[1]->setWorker(w);
  
  struct timespec ts;
  ts.tv_sec = 0;

  //add timeslot 1
  ts = Millis(10); //10ms
  sched->add_slot(ts);

  //add timeslot 2
  ts = Millis(20); //20ms
  sched->add_slot(ts);

  cout << "HSF has been initialized: Periodic TDMA\n";
}

///This function initializes all of the objects with APERIODIC TDMA EXAMPLE
void Simulation::initialize_hierarchical_periodic_tdma() {
  traces = new Trace(this);
  stats = new Statistics(this);

  //Idle should be the first thread to be created
  idle = new Idle(this);


  TDMA *sched0 = new TDMA(this, 1, 0); //id, level
  top_sched = (Scheduler*) sched0;

  TDMA *sched1 = new TDMA(this, 2, 1); //id, level
  TDMA *sched2 = new TDMA(this, 3, 1); //id, level

  top_sched->add_load(sched1);
  top_sched->add_load(sched2);

  disp.push_back((Dispatcher*) new Periodic(this, 4));
  disp.push_back((Dispatcher*) new Periodic(this, 5));
  disp.push_back((Dispatcher*) new Periodic(this, 6));
  disp.push_back((Dispatcher*) new Periodic(this, 7));

  Worker *w;
  BusyWait  *t;
  struct timespec wcet;

  //add 4 workers
  for(uint c=0;c<4;c++) {
    wcet = Millis(5);
    t = new BusyWait(this, disp[c], wcet);
    w = new Worker(this, top_sched, 8+c, busy_wait);
    w->setLoad(t);
    disp[c]->setWorker(w);
    if(c<2) {
      sched1->add_load(w);
    }
    else {
      sched2->add_load(w);
    }
  }

  struct timespec ts;

  for(uint c=0;c<2;c++) {
    ts = Millis(10);
    sched0->add_slot(ts);
    ts = Millis(5);
    sched1->add_slot(ts);
    sched2->add_slot(ts);
  }

  cout << "HSF has been initialized: Hierarchical Periodic TDMA\n";
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
  simulating = 0;  
  cout << "**Done**\n";
  //top_sched->deactivate();

  cout << "Waiting for threads to exit...\n";

  //Join all other threads
  join_all();

  cout << "Saving results...\n";
  //Save statistics to file
  stats->to_file();
  //Save traces to file
  traces->to_file();
  //Save figure to file

  //TODO: figure out why this doesn't work!!!
  //traces->to_figure();
  cout << "All results have been saved!\n";
}

///This function waits for all other thread to join
void Simulation::join_all() {
  //Wait for all dispatchers
#if _DEBUG==0
  cout << "Waiting for dispatchers...\n";
#endif

  for(unsigned int c=0;c<disp.size();c++) {
    if(disp[c] != NULL) {
      disp[c]->join();
    }
  }

#if _DEBUG==0
  cout << "Waiting for idle...\n";
#endif
   
  if(idle != NULL) {
    idle->join();
  }

#if _DEBUG==0
  cout << "Waiting for top_sched...\n";
#endif

  if(top_sched != NULL) {
    cout << "M: All threads .";
    top_sched->join();
    cout << ".. have joined!\n";
  }

  cout << "Joined all!\n";
}

///This function should be called by the Worker constructor to 'register' its id
void Simulation::add_worker_id(unsigned int _id) {
#if _INFO ==1
  cout << "Adding Worker ID: " << _id << endl;
#endif 

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
vector<unsigned int> Simulation::getWorker_id() {
  return worker_id;
}

struct timespec Simulation::getSim_time() {
  return sim_time;
}

void Simulation::setName(string s) {
  name = s;
}

void Simulation::setDuration(struct timespec d) {
  sim_time = d;
}

void Simulation::setTopScheduler(Scheduler *sched) {
  top_sched = sched;
}

void Simulation::addDispatcher(Dispatcher *d) {
  disp.push_back(d);
}
