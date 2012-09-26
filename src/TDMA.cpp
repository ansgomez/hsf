#include "TDMA.h"
//#include "defines.h"

#include "Scheduler.h"
#include "Runnable.h"
#include "Enumerations.h"
#include "Trace.h"

#include <iostream>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/
///Constructor needs pointer to simulation
TDMA::TDMA(Simulation *s, unsigned int id) : Scheduler(s, id) {
  //empty
}

/*********** INHERITED FUNCTIONS ***********/
    
///This function performs the actual scheduling (figuring out the order of execution for its load)
void TDMA::schedule(){
  struct timespec rem;

#if _DEBUG == 1
  cout << "Began scheduling...\n";
#endif

  while (sim->isSimulating() == 1) {

    for(active_index=0; active_index<timeslots.size() && sim->isSimulating()==1; active_index++) {
      sim->getTraces()->add_trace(scheduler, id, sched_start);

      load[active_index]->activate();

#if _DEBUG == 0
      cout << "Activated new Runnable " << active_index << "\n";
#endif
      if(sim->isSimulating() == 1) {
	cout << "sleeping" << timeslots[active_index].tv_sec << ":" << timeslots[active_index].tv_nsec << "\n";
	nanosleep(&timeslots[active_index], &rem);
	cout << "woke up\n";
      }
  
      load[active_index]->deactivate();

      sim->getTraces()->add_trace(scheduler, id, sched_end);
    }
  }
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::new_job(Worker * worker) {
  if (parent != NULL) {
    parent->new_job(worker);
  }
}

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::job_finished(int worker){
  //empty
  cout << " TDMA::job_finished - This should not print!\n";
}


void TDMA::add_slot(struct timespec slot) {
  cout << "Added timeslot, new size: " << timeslots.size() << "\n";
  timeslots.push_back(slot);
}
