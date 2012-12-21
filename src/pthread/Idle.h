#ifndef _IDLE_H
#define _IDLE_H

#include "Thread.h"

class Simulation;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Idle : public Thread {
  /*********** VARIABLES ***********/
 private:
  ///Pointer to simulation
  Simulation *sim;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs simulation pointer
  Idle(Simulation *s);

  /*********** MEMBER FUNCTIONS ***********/
  ///This function contains the flagged loop.
  void run();
  
  /*********** INHERITED FUNCTIONS ***********/
  ///This function is inherited from Thread, and will call the run() function
  void wrapper();
};
#endif
