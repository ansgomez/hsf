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

    for(active_index=0; active_index<(int)timeslots.size() && sim->isSimulating()==1;active_index++) {
 
      //sim->getTraces()->add_trace(scheduler, id, sched_start);
 
      load[active_index]->activate();
 
 #if _DEBUG == 1
      cout << "**Activated new Runnable " << load[active_index]->getId() << "**\n";
 #endif

      //if(sim->isSimulating() == 1)   
      nanosleep(&timeslots[active_index], &rem);

      load[active_index]->deactivate();

      //sim->getTraces()->add_trace(scheduler, id, sched_end);
    }
  }
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::new_job(Worker * worker) {
      cout << "Sched handled Worker " << worker->getId() << "'s new job\n";
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
#if _DEBUG == 1
  cout << "Added timeslot, new size: " << timeslots.size() << "\n";
#endif
 
  timeslots.push_back(slot);
}
