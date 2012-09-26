#include "Worker.h"
//#include "defines.h"

#include "Scheduler.h"
#include "Trace.h"
#include "Task.h"
#include "Simulation.h"
#include "Runnable.h"

#include <stdio.h>

Worker::Worker(Simulation *s, Scheduler *sched, unsigned int _id, _task_load tl) : Runnable(s, _id) {
  sim = s;
  id = _id;
  scheduler = sched;

  sem_init(&wrapper_sem, 0, 0);
}

void Worker::new_job() {
  sem_post(&wrapper_sem);
}

void Worker::wrapper() {
  while (sim->isSimulating() == 1) {
    sem_wait(&wrapper_sem);

    sim->getTraces()->add_trace(worker, id, task_start);

    if(load != NULL) {
      load->fire();    
    }
    else {
      printf("Worker error: load is null!\n");
    }

    sim->getTraces()->add_trace(worker, id, task_end);
  }
}

void Worker::setLoad(Task *t) {
  load = t;
}
