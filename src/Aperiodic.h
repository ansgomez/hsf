#ifndef _APERIODIC_H
#define _APERIODIC_H

#include "Dispatcher.h"

#include <time.h>

class Aperiodic : public Dispatcher {
  private:
    struct timespec release_time;

};
#endif
