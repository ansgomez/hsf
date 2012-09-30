#ifndef _APERIODIC_H
#define _APERIODIC_H

#include "Dispatcher.h"

class Aperiodic : public Dispatcher {
 private:
  struct timespec release_time;

 public:
  ///Constructor needs simulation pointer, and id
  Aperiodic(Simulation *s, unsigned int id);

  /*********** INHERITED FUNCTIONS ***********/
  ///This function was a flagged loop that activates the Worker according to the task periodicity
  void dispatch();

  /*********** GETTER AND SETTER FUNCTIONS ***********/
  void setRelease(struct timespec r);
  struct timespec getRelease();

};
#endif
