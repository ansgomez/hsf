#include "Idle.h"

#include "Simulation.h"
#include "Priorities.h"
#include "Enumerations.h"

#include <iostream>

///The constructor needs the simulation pointer
Idle::Idle(Simulation *s) : Thread(s,0) {
  sim = s;

  type = idle;

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_idle_pr(); 
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}

///This function contains the flagged loop.
void Idle::run() {

#if _INFO == 1
  cout << "Starting Idle thread\n";
#endif

  while (sim->isSimulating() == 1);

#if _INFO == 1
  cout << "Exiting Idle thread\n";
#endif  
}

///This inherited function will only call run()
void Idle::wrapper() {
  run();
}
