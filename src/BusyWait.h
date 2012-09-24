#ifndef _BUSYWAIT_H
#define _BUSYWAIT_H


#include "Task.h"

class BusyWait : public Task {
  private:
    struct timespec WCET;


 public:
    BusyWait(Dispatcher *disp, struct timespec wcet);

    void fire();
};
#endif

