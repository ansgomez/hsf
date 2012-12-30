#include "core/ResourceAllocator.h"

#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs runnable's id
ResourceAllocator::ResourceAllocator(unsigned int _id) : Runnable(_id) {
  //empty
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///This is the pthread's wrapper function
void ResourceAllocator::wrapper() {
  cout << "ResourceAllocator::wrapper - This should not print!\n";
}
  
/**** FROM RUNNABLE ****/

///This function rewrites the activate method to activate both the scheduler as well as its load
void ResourceAllocator::activate() {
  cout << "ResourceAllocator::activate - This should not print!\n";
}

///This function rewrites the deactivate method both the scheduler as well as its load
void ResourceAllocator::deactivate() {
  cout << "ResourceAllocator::deactivate - This should not print!\n";
}

/*********** MEMBER FUNCTIONS ***********/
  
///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void ResourceAllocator::new_job(Runnable *r) {
  cout << "ResourceAllocator::new_job - This should not print!\n";
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void ResourceAllocator::renew_job(Runnable* r) {
  cout << "ResourceAllocator::renew_job - This should not print!\n";
}

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void ResourceAllocator::job_finished(unsigned int id) {
  cout << "ResourceAllocator::finished - This should not print!\n";
}
