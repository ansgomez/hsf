#include "Simulation.h"

#include "Dispatcher.h"
#include "Scheduler.h"
#include "Worker.h"
#include "Periodic.h"
#include "Parser.h"
#include "Priorities.h"
#include "Trace.h"
#include "TDMA.h"
#include "TimeUtil.h"
#include "BusyWait.h"
#include "Task.h"
#include "Idle.h"

#include <iostream>
#include <unistd.h>
#include <stdio.h>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Simulation::Simulation(string xml_path, int cpu, string nm) {
  struct sched_param param = {0};
  name = nm;

  sim_time.tv_sec = 1;
  sim_time.tv_nsec = 0;

  simulating = 0;

  //  cout << "Loading xml file...\n";

  //Set main process parameters
  param.sched_priority = Priorities::get_main_pr(); //highest priority
  if ((sched_setscheduler(0, SCHED_FIFO, &param) != 0)) {
    cout << "Run with root\n";
    pthread_exit(0);
  }

  if (cpu==1)
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

///This function initializes all of the objects
void Simulation::initialize() {
  traces = new Trace(this);

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
  wcet.tv_nsec = 1000000; //1ms
  t = new BusyWait(disp[0], wcet);
  w = new Worker(this, top_sched, 4, busy_wait);
  w->setLoad(t);
  top_sched->add_load(w);
  disp[0]->setWorker(w);

  //add second worker
  wcet.tv_nsec = 2000000; //2ms
  t = new BusyWait(disp[1], wcet);
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
}


void Simulation::simulate() {
  struct timespec rem;
  cout << "**Simulating**\n" ;

  simulating = 1;  
  TimeUtil::setOffset();

  disp[0]->activate();
  disp[1]->activate();

  top_sched->activate();

  nanosleep(&sim_time, &rem);

cout << "HELLO\n";

  simulating = 0;  
  top_sched->deactivate();

  cout << "Stopped simulation\n";

  traces->to_file();

  cout << "Saved simulation results...\n";
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
