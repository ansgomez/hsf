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
  sem_init(&schedule_sem, 0, 0); //mutex semaphore
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

	aux = timeA + time_slot;

#if _DEBUG==1
	if(level==0) {
	  cout << "TopSched: timeA - " << TimeUtil::convert_us(timeA, relative) << " | time_slot - " << TimeUtil::convert_us(time_slot) << " | aux - " << TimeUtil::convert_us(aux, relative) << " @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
	}
#endif

	ret = sem_timedwait(&preempt_sem, &aux);
	err_no = errno;
	sem_post(&timing_sem);

#if _DEBUG==1
	if(level==1) {
	  cout << "Level 1 exited timedwait! (returned " << ret << ") @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
	}
#endif

#if _DEBUG==1
	if(level==0) {
	  cout << "Top Level exited timedwait! (returned " << ret << ") @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
	}
#endif

	//If simulation ended while asleep, break
	if(sim->isSimulating()==0) {
	  sem_post(&schedule_sem);
	  break;
	}

#if _DEBUG==1
	if(level==0) {
	  cout << "Top Level posted schedule_sem! @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
	}
#endif	

	if(load[active_index] != NULL) {
	  load[active_index]->deactivate();
	}

#if _DEBUG==1
	if(level==0) {
	  cout << "TopSched: return value - " << ret << " @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
	}
#endif

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
	    //exit = true;
	    //sem_post(&schedule_sem);
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

#if _INFO == 1
  cout << "Exiting schedule function - Thread " << id << endl;
#endif
}//end of function

///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
void TDMA::activate() {
  if(state == activated) {
    cout << "TDMA::activate - already activated!\n";
  }

  sem_wait(&activation_sem);

  sem_post(&schedule_sem);
    
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_sched_pr(level); 
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);

  state = activated;
    
  sem_post(&activation_sem);
}

///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void TDMA::deactivate() {
  int sts, err_no;

  if(state == deactivated) {
    cout << "TDMA::deactivate - already deactivated!\n";
  }

  sem_wait(&activation_sem);

#if _DEBUG==1
  if(level==1) {
  cout << "Top Level woke up at activation_sem! @t=" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
}
#endif

  sts = sem_trywait(&timing_sem);

#if _DEBUG==1
  if(level==1) {
    cout << "Top Level activation_sem return " << sts << endl;
  }
#endif

  //If the scheduler isn't timing, no need to do anything (just wait for sem to be freed)
  if (sts == 0) {
    //empty
    sts = 0;
    sem_post(&timing_sem);
  }
  else { 
    //If the scheduler has timing_sem locked, then it must be preempted
      sem_post(&preempt_sem);
      //errno = -1;
    } 

#if _DEBUG==1
    if(level==1) {
      cout << "Top Level dectivation @t-" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
    }
#endif

    sem_wait(&schedule_sem);

#if _DEBUG==1
    if(level==1) {
      cout << "Top Level dectivation after sem_wait @t-" << TimeUtil::convert_us(TimeUtil::getTime(), relative) << endl;
    }
#endif

    //now decrease the priority
    pthread_getschedparam(thread, &policy, &thread_param);
    thread_param.sched_priority = Priorities::get_inactive_pr(); //active priority
    pthread_setschedparam(thread, SCHED_FIFO, &thread_param);  

    state = deactivated;
    sem_post(&activation_sem);
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
