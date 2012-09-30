#ifndef _BUSYWAIT_H
#define _BUSYWAIT_H

#include "Task.h"

class BusyWait : public Task {
  private:
    struct timespec WCET;


 public:
    BusyWait(Simulation *s, Dispatcher *disp, struct timespec wcet);

    void fire();
};
#endif

