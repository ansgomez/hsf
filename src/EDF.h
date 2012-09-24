#ifndef _EDF_H
#define _EDF_H


#include "Scheduler.h"

class Deadline;

class EDF : public Scheduler {
  private:
    Vector<Deadline> deadlines;

};
#endif
