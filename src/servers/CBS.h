#ifndef _CBS_H
#define _CBS_H


#include "core/Scheduler.h"
#include "core/Server.h"

class Deadline;

class CBS : public Scheduler, public Server {
  private:
    Vector<Deadline> deadlines;


  public:
    void replenish();

    void serve();

};
#endif
