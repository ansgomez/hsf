#include "TDMA.h"
//#include "defines.h"

#include "Scheduler.h"
#include "Runnable.h"
#include "Enumerations.h"
#include "Operators.h"
#include "Trace.h"
#include "Priorities.h"
#include "TimeUtil.h"

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
  sem_init(&timing_sem, 0, 1); //mutex semaphore
  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&preempt_sem, 0, 0); //sem used as signal
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
 
#if _DEBUG == 1
	cout << "**S: " << id << " activated new Runnable " << load[active_index]->getId() << " for " << TimeUtil::convert_us(time_slot)/1000 << " ms @t="<< TimeUtil::convert_us(TimeUtil::getTime(), relative) << " **\n";
#endif
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

#if _DEBUG == 1
	cout << "**S: " << id << " deactivated new Runnable " << load[active_index]->getId() << " @t="<< TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
#endif

	//If the call timedout
	if(ret == -1) {
	  //if timeslot was exhausted, pass on to the next time slot
	  if(errno == ETIMEDOUT) {
#if _DEBUG == 1
	    cout << "S: " << id << " - Timeslot expired! @t="<< TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
#endif
	    sem_post(&schedule_sem);
	    exit = true;
	  }
	  else if (errno==EINVAL) {
	    cout << "TDMA::schedule: EINVAL ERROR\n";
	  }
	  else if (errno==EAGAIN) {
	    //exit = true;
	    //sem_post(&schedule_sem);
	    //cout << "TDMA::schedule: EAGAIN ERROR\n";
	  }
	  else {
	    cout << "TDMA::schedule: semaphore error (" << errno << ") - " << strerror(errno) << "\n";
	  }
	}
	//If the call received a signal, it is being deactivated
	else {
#if _DEBUG == 1
	cout << "S: " << id << " - Decreasing time_slot @t="<< TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
#endif
	  clock_gettime(CLOCK_REALTIME, &timeB);
	  time_slot = time_slot - (timeB-timeA);
	  sem_post(&schedule_sem);
        }
      }//end of while(exit==false)
    }//end of foreach(timeslot)

    active_index = -1;
  }//end of while(simulating)

#if _INFO == 1
  cout << "Exiting schedule function - Thread " << id << endl;
#endif
}//end of function

///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
void TDMA::activate() {
  // if(state == deactivated) 
    {
    sem_wait(&activation_sem);

    sem_post(&schedule_sem);
    
    pthread_getschedparam(thread, &policy, &thread_param);
    thread_param.sched_priority = Priorities::get_sched_pr(level); //server priority
    pthread_setschedparam(thread, SCHED_FIFO, &thread_param);

    state = activated;
    
    sem_post(&activation_sem);
  }
}

///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void TDMA::deactivate() {
  int sts, err_no;

  // if(state == activated)
    {
    sem_wait(&activation_sem);

    sts = sem_trywait(&timing_sem);
    //err_no = errno;

    //If the scheduler isn't timing, no need to do anything (just wait for sem to be freed)
    if (sts == 0) {
      //empty
      sts = 0;
      sem_post(&timing_sem);
    }
    else { //if (err_no == EAGAIN) {
      //If the scheduler has timing_sem locked, then it must be preempted
      sem_post(&preempt_sem);
      //errno = -1;
    } /*
    else {
      perror("sem_trywait() failure");
      } */

    sem_wait(&schedule_sem);

    //now decrease the priority
    pthread_getschedparam(thread, &policy, &thread_param);
    thread_param.sched_priority = Priorities::get_inactive_pr(); //active priority
    pthread_setschedparam(thread, SCHED_FIFO, &thread_param);  

    state = deactivated;
    sem_post(&activation_sem);
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
void TDMA::job_finished(unsigned int worker_id){
  cout << " Sched handled Worker " << worker << "'s finished job!\n";
}

/********************* MEMBER FUNCTIONS *********************/

///This function adds a timeslot to the scheduler. They are assigned in the same order as the load is defined
void TDMA::add_slot(struct timespec slot) {
#if _DEBUG == 1
  cout << "Added timeslot, new size: " << timeslots.size() << "\n";
#endif
 
  timeslots.push_back(slot);
}
