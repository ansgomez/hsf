#include "TDMA.h"
//#include "defines.h"

#include "Scheduler.h"
#include "Runnable.h"
#include "Enumerations.h"
#include "Operators.h"
#include "Trace.h"
#include "Priorities.h"

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

  timing = 0;
  sem_init(&schedule_sem, 0, 1); //mutex semaphore
  sem_init(&preempt_sem, 0, 0); //sem used as signal
}

/*********** INHERITED FUNCTIONS ***********/
    
///This function performs the actual scheduling (figuring out the order of execution for its load)
void TDMA::schedule(){
  struct timespec timeA, timeB, time_slot, aux;
  bool exit = false;
  int ret;

#if _DEBUG == 1
  cout << "Began scheduling...\n";
#endif

  while (sim->isSimulating() == 1) {

    for(active_index=0; active_index<(int)timeslots.size() && sim->isSimulating()==1; active_index++) {

      time_slot = timeslots[active_index];

      exit = false;

      while(exit == false &&  sim->isSimulating()==1) {

	sem_wait(&schedule_sem);

	//start timing
	clock_gettime(CLOCK_REALTIME, &timeA);

	load[active_index]->activate();
 
#if _DEBUG == 1
	cout << "**Activated new Runnable " << load[active_index]->getId() << " for " << time_slot.tv_sec << "." << time_slot.tv_nsec << "**\n";
#endif
	aux = timeA + time_slot;
	timing = 1;
	ret = sem_timedwait(&preempt_sem, &aux);
	timing = 0;

	load[active_index]->deactivate();

	//If the call timedout
	if(ret == -1) {
	  //if timeslot was exhausted, pass on to the next time slot
	  if(errno == ETIMEDOUT) {
#if _DEBUG == 1
	    cout << "Timeslot expired!\n";
#endif
	    sem_post(&schedule_sem);
	    exit = true;
	  }
	  else if (errno==EINVAL) {
	    cout << "TDMA::schedule: EINVAL ERROR\n";
	  }
	  else {
	    cout << "TDMA::schedule: semaphore error\n";
	  }
	}
	//If the call received a signal, it is being deactivated
	else {
	  //cout << "Decreasing time_slot\n";
	  clock_gettime(CLOCK_REALTIME, &timeB);
	  time_slot = time_slot - (timeB-timeA);
	  sem_post(&schedule_sem);
        }
      }//end of while(exit==false)
    }//end of foreach(timeslot)
  }//end of while(simulating)
}//end of function

///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
void TDMA::activate() {
  sem_post(&schedule_sem);

  //SCHED activation is not traced
  //sim->getTraces()->add_trace(scheduler, id, sched_activate);
 
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_sched_pr(level); //server priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
 
  //if there was an active load, reactivate it
  if(active_index != -1) {
    load[active_index]->activate();
  }
}

///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void TDMA::deactivate() {
  if(timing == 1) {
    sem_post(&preempt_sem);
  }

  sem_wait(&schedule_sem);

  //if there was an active load, deactivate it
  if(active_index != -1) {
    load[active_index]->deactivate();
  }

  //SCHED deactivation is not traced
  //sim->getTraces()->add_trace(scheduler, id, sched_deactivate);

  //now decrease the priority
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);  

}


///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::new_job(Worker * worker) {
  cout << "Sched handled Worker " << worker->getId() << "'s new job\n";
  if (parent != NULL) {
    parent->new_job(worker);
  }
}


///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::job_finished(unsigned int worker_id){
  cout << " Sched handled Worker " << worker << "'s finished job!\n";
}


void TDMA::add_slot(struct timespec slot) {
#if _DEBUG == 1
  cout << "Added timeslot, new size: " << timeslots.size() << "\n";
#endif
 
  timeslots.push_back(slot);
}
