#ifndef _PERIODICJITTER_H
#define _PERIODICJITTER_H


#include "Dispatcher.h"

class PeriodicJitter : public Dispatcher {
  /*********** VARIABLES ***********/
 private:
  ///The dispatcher's period
  struct timespec period;

  ///The dispatcher's jitter
  struct timespec jitter;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs simulation pointer, and id
  PeriodicJitter(Simulation *s, unsigned int id);

  /*********** INHERITED FUNCTIONS ***********/
  ///This function was a flagged loop that activates the Worker according to the task periodicity
  void dispatch();

  /*********** GETTER AND SETTER FUNCTIONS ***********/
  ///This function sets the period
  void setPeriod(struct timespec p);
  ///This function sets the jitter
  void setJitter(struct timespec j);
  ///This function returns the period
  struct timespec getPeriod();
};
#endif
