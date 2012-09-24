#include "TDMA.h"

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
  unsigned int c;

#if _DEBUG == 1
  cout << "Began scheduling...\n";
#endif

  while (sim->isSimulating() == 1) {
    for(c=0;c<timeslots.size() && sim->isSimulating()==1;c++) {

      sim->getTraces()->add_trace(scheduler, sched_id, sched_start);

      load[c]->activate();

#if _DEBUG == 0
      cout << "Activated new Runnable " << c << "\n";
#endif
      nanosleep(&timeslots[c], &rem);
  
      load[c]->deactivate();

      sim->getTraces()->add_trace(scheduler, sched_id, sched_end);
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
