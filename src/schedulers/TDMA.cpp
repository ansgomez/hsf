#include "schedulers/TDMA.h"

#include "core/Scheduler.h"
#include "core/Runnable.h"
#include "util/Enumerations.h"
#include "util/Operators.h"
#include "results/Trace.h"
#include "pthread/Priorities.h"
#include "util/TimeUtil.h"

#include <time.h>
#include <iostream>
#include <errno.h>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/
///Constructor needs pointer to simulation
TDMA::TDMA(Simulation *s, unsigned int id, int level) : Scheduler(s, id, level) {
#if _INFO==1
  cout << "Creating TDMA with ID: " << id << endl;
#endif

  sem_init(&schedule_sem, 0, 0); //mutex semaphore -> initialized to 0 because of activate()
  sem_init(&timing_sem, 0, 1); //mutex semaphore
  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&preempt_sem, 0, 0); //sem used as signal

  state = deactivated;
  active_index = -1;
  sched_type = tdma;
}

/*********** INHERITED FUNCTIONS ***********/
    
///This function performs the actual scheduling (figuring out the order of execution for its load)
void TDMA::schedule(){
  struct timespec timeA, timeB, time_slot, aux;
  bool exit = false;
  int ret, err_no;
  active_index = -1;

#if _DEBUG == 1
  cout << "Began scheduling...\n";
#endif

  while (sim->isSimulating() == 1) {
    
    for(active_index=0; active_index<(int)timeslots.size() && sim->isSimulating()==1; active_index++) {

      time_slot = timeslots[active_index];

      exit = false;

      while(exit == false && sim->isSimulating()==1) {

	sem_wait(&schedule_sem);

	//If simulation ended while asleep, break
	if(sim->isSimulating()==0) {
	  sem_post(&schedule_sem);
	  break;
	}

	//start timing
	sem_wait(&timing_sem);

	clock_gettime(CLOCK_REALTIME, &timeA);

	if(load[active_index] != NULL) {
	  load[active_index]->activate();
	}

	aux = timeA + time_slot;

	ret = sem_timedwait(&preempt_sem, &aux);
	err_no = errno;
	sem_post(&timing_sem);

	//If simulation ended while asleep, break
	if(sim->isSimulating()==0) {
	  sem_post(&schedule_sem);
	  break;
	}

	if(load[active_index] != NULL) {
	  load[active_index]->deactivate();
	}

	//If the call timedout
	if(ret == -1) {
	  //if timeslot was exhausted, pass on to the next time slot
	  if(err_no == ETIMEDOUT) {
	    sem_post(&schedule_sem);
	    exit = true;
	  }
	  else if (err_no==EINVAL) {
	    cout << "TDMA::schedule: EINVAL ERROR\n";
	  }
	  else if (err_no==EAGAIN) {
	    cout << "TDMA::schedule: EAGAIN ERROR\n";
	  }
	  else {
	    cout << "TDMA::schedule: semaphore error (" << errno << ") - " << strerror(errno) << "\n";
	  }
	}
	//If the call received a signal, it is being deactivated
	else {
	  clock_gettime(CLOCK_REALTIME, &timeB);
	  time_slot = time_slot - (timeB-timeA);
	  
	  if(time_slot < Millis(0)) {
	    exit = true;
	  }

	  sem_post(&schedule_sem);
        }
      }//end of while(exit==false)
    }//end of foreach(timeslot)

    active_index = -1;
  }//end of while(simulating)

}//end of function

///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
void TDMA::activate() {
  if(state == activated) {
    cout << "TDMA::activate error - already activated!\n";
  }

  sem_wait(&activation_sem);

  sem_post(&schedule_sem);
    
  setPriority(Priorities::get_sched_pr(level));

  state = activated;
    
  sem_post(&activation_sem);
}

///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void TDMA::deactivate() {
  int sts, err_no;

  if(state == deactivated) {
    cout << "TDMA::deactivate error - already deactivated!\n";
  }

  sem_wait(&activation_sem);

  sts = sem_trywait(&timing_sem);

  //If the scheduler isn't timing, no need to do anything (just wait for sem to be freed)
  if (sts == 0) {
    sem_post(&timing_sem);
  }
  else { 
    //If the scheduler has timing_sem locked, then it must be preempted
    sem_post(&preempt_sem);
  } 

  sem_wait(&schedule_sem);

  //now decrease the priority
  setPriority(Priorities::get_inactive_pr());
  
  state = deactivated;
  sem_post(&activation_sem);
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::new_job(Worker * worker) {
  //cout << "Sched handled Worker " << worker->getId() << "'s new job\n";
  if (parent != NULL) {
    parent->new_job(worker);
  }
}

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::job_finished(unsigned int worker_id){
  //cout << " Sched handled Worker " << worker << "'s finished job!\n";
  if (parent != NULL) {
    parent->job_finished(worker);
  }
}

///This function rewrites the join method to account for the scheduler's load (they are all joined)
void TDMA::join() {
  for(unsigned int c=0; c<load.size(); c++) {
    if( load[c] != NULL) {
      load[c]->join();
    }
  }
}

/*********** MEMBER FUNCTIONS ***********/

///This function adds a load to the scheduler (could be another scheduler, or a worker)
void Scheduler::add_load(Runnable *new_load) {
  load.push_back(new_load);
}

///This function adds a timeslot to the scheduler. They are assigned in the same order as the load is defined
void TDMA::add_slot(struct timespec slot) {
  timeslots.push_back(slot);
}
