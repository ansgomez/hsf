#ifndef _RR_H
#define _RR_H


#include "Scheduler.h"

class P_RR : public Scheduler {
  private:
    struct timespec timeslice;

};
#endif
