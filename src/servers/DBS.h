#ifndef _DBS_H
#define _DBS_H


#include "core/Scheduler.h"

class Task;

class DBS : public Scheduler {
  private:
    Vector<Task> task_queue;

};
#endif
