#ifndef _PERIODICJITTER_H
#define _PERIODICJITTER_H


#include "Dispatcher.h"

class PeriodicJitter : public Dispatcher {
  private:
    struct timespec period;

    struct timespec jitter;

};
#endif
