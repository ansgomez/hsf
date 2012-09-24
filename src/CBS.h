#ifndef _CBS_H
#define _CBS_H


#include "Scheduler.h"

class Deadline;

class CBS : public Scheduler {
  private:
    Vector<Deadline> deadlines;

};
#endif
