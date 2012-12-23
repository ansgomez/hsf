#ifndef _APERIODIC_H
#define _APERIODIC_H

#include "core/Dispatcher.h"

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

class Aperiodic : public Dispatcher {
  /*********** VARIABLES ***********/
 private:
  ///This parameter specifies the release time of the aperiodic dispatcher
  struct timespec release_time;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs simulation pointer, and id
  Aperiodic(Simulation *s, unsigned int id);

  /*********** INHERITED FUNCTIONS ***********/
  ///This function was a flagged loop that activates the Worker according to the task periodicity
  void dispatch();

  /*********** GETTER AND SETTER FUNCTIONS ***********/
  ///This function sets the release time for the aperiodic dispatcher
  void setRelease(struct timespec r);

  ///This function returns the release time
  struct timespec getRelease();
};
#endif
