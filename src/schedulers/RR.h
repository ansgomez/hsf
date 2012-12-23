#ifndef _RR_H
#define _RR_H


#include "core/Scheduler.h"

class P_RR : public Scheduler {
  private:
    struct timespec timeslice;

};
#endif
