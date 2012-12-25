#ifndef _IDLE_H
#define _IDLE_H

#include "pthread/Thread.h"

class Simulation;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Idle : public Thread {

 private:

  /*********** VARIABLES ***********/

  ///Pointer to simulation
  Simulation *sim;

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs simulation pointer
  Idle(Simulation *s);

  /*********** INHERITED FUNCTIONS ***********/
  
  /**** FROM THREAD ****/
  
  ///This function is inherited from Thread, and will call the run() function
  void wrapper();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function contains the flagged loop.
  void run();
  
};
#endif
