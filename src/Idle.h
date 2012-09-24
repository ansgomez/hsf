#ifndef _IDLE_H
#define _IDLE_H

#include "Thread.h"

class Simulation;

class Idle : public Thread {
 private:

  Simulation *sim;

  public:
  ///Constructor needs simulation pointer
    Idle(Simulation *s);

    ///This function contains the flagged loop.
    void run();
    
    void wrapper();
};
#endif
