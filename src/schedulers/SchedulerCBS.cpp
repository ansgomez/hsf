#include "SchedulerCBS.h"

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation, id, and its level
SchedulerCBS::SchedulerCBS(unsigned int id, int level) : Scheduler(id, level) {

}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM RUNNABLE  ****/
///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
void SchedulerCBS::activate() {

}


///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void SchedulerCBS::deactivate() {

}

/**** FROM RESOURCEALLOCATOR  ****/

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void SchedulerCBS::newJob(Runnable *ojb) {

}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void SchedulerCBS::updateRunnable(Runnable* r) {

}

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void SchedulerCBS::finishedJob(unsigned int runnable_id) {

}

/**** FROM SCHEDULER  ****/
///This function performs the actual scheduling (figuring out the order of execution for its load)
void SchedulerCBS::schedule() {

}
