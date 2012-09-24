#ifndef _PERIODIC_H
#define _PERIODIC_H

#include "Dispatcher.h"

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Periodic : public Dispatcher {
 private:
  struct timespec period;

 public:
  ///Constructor needs simulation pointer, and id
  Periodic(Simulation *s, unsigned int id);

  /*********** INHERITED FUNCTIONS ***********/
  ///This function was a flagged loop that activates the Worker according to the task periodicity
  void dispatch();
};
#endif
