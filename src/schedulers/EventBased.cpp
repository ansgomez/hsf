#include "schedulers/EventBased.h"

#include "core/Criteria.h"
#include "core/RunnableQueue.h"

#include <iostream>
#include <time.h>

#define _INFO 0
#define _DEBUG 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
EventBased::EventBased(unsigned int _id, int level) : Scheduler(_id, level) {
  #if _INFO==1
  cout << "Creating EventBased with ID: " << id << endl;
  #endif

  //Subclasses should initialize the activeQueue
  //variable depending on their schedulable criteria

  //Initialize semaphores
  sem_init(&activation_sem, 0, 1);  //mutex semaphore
  sem_init(&event_sem, 0, 0);       //sem used as signal
  sem_init(&finishedjob_sem, 0, 1); //mutex semaphore
  sem_init(&newjob_sem, 0, 1);      //mutex semaphore
  sem_init(&update_sem, 0, 1);      //mutex semaphore
  sem_init(&schedule_sem, 0, 1);    //mutex semaphore
}

//TODO: DESTRUCTOR

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD  ****/

///This function redefines Thread::join() to take into account the EventBased unblocking mechanism...
void EventBased::join() {

  #if _DEBUG==1
  cout << "Attempting to join EventBased\n";
  #endif

  sem_post(&event_sem);
  sem_post(&schedule_sem);
  sem_post(&activation_sem);
  sem_post(&newjob_sem);
  sem_post(&finishedjob_sem);
  sem_post(&update_sem);

  if(parent!=NULL) {
    parent->join();
  }

  //after posting to all blocking sems, 
  join2();

}

/**** FROM RUNNABLE  ****/

///This function rewrites the activate method to 'activate' both the scheduler as well as its load - this runs in the higher level scheduler thread
void EventBased::activate() {

  #if _DEBUG==1
  cout << "Activating EventBased...\n";
  #endif

  if(state == activated) {
    cout << "EventBased::activate() error - already activated!\n";
  }

  sem_wait(&activation_sem);
    sem_wait(&schedule_sem);

      setPriority(Priorities::get_sched_pr(level));
      state = activated;

      //Activate current head of active_queue (if any) only when there are 
      //no new jobs (since the current job might have to be preempted)
      //sem_wait(&newjob_sem);

        //If there are no new jobs pending
        if(newJobQueue.size() == 0) {
          //activate current head of queue
          if(activeQueue->peek_front() != NULL) {
            activeQueue->peek_front()->activate();
          }
        }
        //if there are new jobs, wait for scheduler to process the new jobs himself
	//sem_post(&newjob_sem);
    sem_post(&schedule_sem); 
  sem_post(&activation_sem);

  #if _DEBUG==1
  cout << "Activated EventBased\n";
  #endif
}

///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void EventBased::deactivate() {
  if(state == deactivated) {
    cout << "EventBased::deactivate() error - already deactivated!\n";
  }

  sem_wait(&activation_sem);
    sem_wait(&schedule_sem);
      //Deactivate currently active job (if any)
      if(activeQueue->peek_front() != NULL) {
        activeQueue->peek_front()->deactivate();
      }
      //Decrease the priority
      setPriority(Priorities::get_inactive_pr());
      state = deactivated;
    sem_post(&schedule_sem);
  sem_post(&activation_sem);

  #if _DEBUG==1
  cout << "Dectivated EventBased\n";
  #endif
}

/**** FROM RESOURCEALLOCATOR  ****/

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void EventBased::finishedJob(unsigned int runnable_id) {

  #if _DEBUG==1
  cout << "EventBased::finishedJob() is waiting for schedule_sem\n";
  #endif

  sem_wait(&schedule_sem);
  //sem_wait(&finishedjob_sem);
      #if _DEBUG==1
      cout << "EventBased::finishedJob() is processing finishedJob\n";
      #endif
      finishedJobQueue.push_back(runnable_id); //Add id to finishedJobQueue
      //sem_post(&finishedjob_sem);

    //This will only be reached when all queues were empty
    //This meands that scheduler needs to be signaled!
    #if _DEBUG==1
    cout << "EventBased::finishedJob() - Registering finishedJob & event...\n";
    #endif

    //Register event with parent
    if(parent!=NULL) {
      parent->finishedJob(id);
    }
    else if (level != 0) {
      cout << "EventBased::finishedJob() - non-top level entity has null parent!\n";
    }
    
    //Protecting posts to event_sem with sched_sem assures one event handled 
    //per post in the scheduler, otherwise, multiple jobs could be handled from just one post
    sem_post(&event_sem);//Register event with this scheduler

    #if _DEBUG==1
    cout << "finishedJob is done!\n";
    #endif
  sem_post(&schedule_sem);
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution. This is executed by the worker thread itself (always of a lower priority than its scheduler)
void EventBased::newJob(Runnable* r) {

  #if _DEBUG==1
  cout << "EventBased::newJob() is waiting for schedule_sem\n";
  #endif

  sem_wait(&schedule_sem);
  //sem_wait(&newjob_sem);
      newJobQueue.push_back(r);//Add new arrival
      //sem_post(&newjob_sem); 

    #if _DEBUG==1
    cout << "EventBased::newJob() registered newJob and event!\n";
    #endif

    //Set the scheduler's criteria equal to its load's criteria
    //TODO: enforce some bounds on the load's criteria before assignment
    criteria = r->getCriteria();
 
    //Notify parent of new head
    if(parent!=NULL) {
      parent->newJob(this);
    }
    else if (level != 0) {
      cout << "EventBased::newJob() - non-top level entity has null parent!\n";
    }

    #if _DEBUG==1
    cout << "EventBased::newJob() is done!\n";
    #endif

    //Alert scheduler of event 
    sem_post(&event_sem); //->posting to event_sem must be protected by sched_sem!
  sem_post(&schedule_sem);
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void EventBased::updateRunnable(Runnable* r) {
  #if _DEBUG==1
  cout << "EventBased::updateRunnable() is waiting for schedule_sem\n";
  #endif

  sem_wait(&schedule_sem);
    //add to updateQueue
    //sem_wait(&update_sem);
      updateQueue.push_back(r);
      //sem_post(&update_sem); 
    //register event
    sem_post(&event_sem);
  sem_post(&schedule_sem);
}

/**** FROM SCHEDULER  ****/

///This function performs the actual scheduling (figuring out the order of execution for its load)
void EventBased::schedule() {
  Runnable* currentRunnable;

  while(Simulation::isSimulating()) {

    //Wait for an event to ocurr
    sem_wait(&event_sem);

    #if _DEBUG==1
    cout << "EventBased::schedule() is handling an event!\n";
    #endif

    //If simulation ended while blocked, break
    if( !Simulation::isSimulating() )  break;
    
    sem_wait(&schedule_sem);

      #if _DEBUG==1
      cout << "EventBased::schedule() is processing an event: ";
      cout << newJobQueue.size() << ":" << finishedJobQueue.size() << ":" << updateQueue.size() << "\n";
      #endif

      //Deactivate currently active job (if any) in order to process new/finished jobs
      currentRunnable = activeQueue->peek_front();
      if(currentRunnable != NULL) {
        currentRunnable->deactivate();
      }

      //sem_wait(&update_sem);
      //sem_wait(&newjob_sem);
      //sem_wait(&finishedjob_sem);

      /***** handle updates *****/
      if(updateQueue.size() > 0) {
        #if _DEBUG==1
	cout << "EventBased::schedule() is handling a runnable update!\n";
        #endif
	Runnable* r = updateQueue.front();
	activeQueue->deleteRunnable(r->getId());//erase from activeQueue
	activeQueue->insertRunnable(r);//insert into activeQueue with updated criteria
	updateQueue.pop_front();//erase from updateQueue
	
	if(parent!=NULL) {
	  parent->updateRunnable(this);
	}
	else if (level != 0) {
	  cout << "EventBased::schedule() - non-top level entity has null parent!\n";
	}
      }
      /***** handle new jobs *****/
      else if(newJobQueue.size()>0) {
        #if _DEBUG==1
        cout << "EventBased::schedule() is handling a new job!\n";
        #endif
        activeQueue->insertRunnable(newJobQueue.front());//insert head of newJobQueue
        newJobQueue.pop_front();//erase head of newJobQueue
      }
      /***** handle finished jobs *****/
      else if(finishedJobQueue.size() > 0) {
        #if _DEBUG==1
        cout << "EventBased::schedule() is handling a finished job!\n";
        #endif
        activeQueue->deleteRunnable(finishedJobQueue.front());//erase from activeQueue
        finishedJobQueue.pop_front();//erase from finishedJobQueue
      }

      //activate the (possibly new) head of activeQueue
      if(activeQueue->getSize()>0) {
	//cout << "EventBased::schedule() - activating: " << activeQueue->peek_front()->getId() << endl;
	activeQueue->peek_front()->activate();
      }

      //release all sems
      //sem_post(&update_sem);
      //sem_post(&newjob_sem);
      //sem_post(&finishedjob_sem);

      #if _DEBUG==1
      cout << "EventBased::schedule() is releasing sem...\n";
      #endif

    //Release sched_sem
    sem_post(&schedule_sem);
  }
}

/*********** MEMBER FUNCTIONS ***********/

///This function compares a Criteria object with the current head of the active queue to determine whether the new Criteria is "greater" -> this could mean a higher priority, or earlier deadline, as defined by each subclass of the EventBased class.
bool EventBased::greaterCriteria(Criteria* newCriteria) {
  cout << "EventBased::greaterCriteria() - error! This should not print!\n";
  return false;
}
