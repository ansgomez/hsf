#include "core/Intermediary.h"

#include "core/Simulation.h"

#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs runnable's id
Intermediary::Intermediary(unsigned int _id) : Runnable(new Simulation("",1,""), _id) {
  //empty
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///This function rewrites the join method to account for the scheduler's load (they are all joined)
void Intermediary::join() {
  cout << "Intermediary::join - This should not print!\n";
}

///This is the pthread's wrapper function
void Intermediary::wrapper() {
  cout << "Intermediary::wrapper - This should not print!\n";
}
  
/**** FROM RUNNABLE ****/

///This function rewrites the activate method to activate both the scheduler as well as its load
void Intermediary::activate() {
  cout << "Intermediary::activate - This should not print!\n";
}

///This function rewrites the deactivate method both the scheduler as well as its load
void Intermediary::deactivate() {
  cout << "Intermediary::deactivate - This should not print!\n";
}

/*********** MEMBER FUNCTIONS ***********/
  
///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void Intermediary::new_job(Runnable *r) {
  cout << "Intermediary::new_job - This should not print!\n";
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void Intermediary::renew_job(Runnable* r) {
  cout << "Intermediary::renew_job - This should not print!\n";
}

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void Intermediary::job_finished(unsigned int id) {
  cout << "Intermediary::finished - This should not print!\n";
}
