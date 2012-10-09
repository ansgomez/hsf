#ifndef _BUSYWAIT_H
#define _BUSYWAIT_H

#include "Task.h"

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class BusyWait : public Task {
  /*********** VARIABLES ***********/
  private:
  ///This paremeter specifies the worst-case execution time for the busy wait loop
  struct timespec WCET;

  /*********** CONSTRUCTOR ***********/
 public:
    ///The constructor requires a pointer to the simulation, its own dispatcher, and the WCET
    BusyWait(Simulation *s, Dispatcher *disp, struct timespec wcet);


  /*********** INHERITED FUNCTIONS ***********/
    //This function will perform one task with execution time WCET
    void fire();
};
#endif

