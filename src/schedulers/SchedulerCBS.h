#ifndef _CBS_SCHED_H
#define _CBS_SCHED_H


#include "core/Scheduler.h"

class Deadline;
class CBS_Queue;

class CBS_SCHED : public Scheduler {
  public:
    void updateDeadline(const unsigned int & id, const Deadline & deadline);

    void newJob(const unsigned int & id, const Deadline & deadline);

    void jobFinished(const unsigned int & id);

    void schedule();


  private:
    CBS_Queue * queue;

};
#endif
