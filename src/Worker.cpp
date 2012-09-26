#include "Worker.h"
//#include "defines.h"

#include "Scheduler.h"
#include "Trace.h"
#include "Task.h"
#include "Simulation.h"
#include "Runnable.h"
#include "Priorities.h"

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


///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
void Worker::activate() {

  sim->getTraces()->add_trace(worker, id, sched_start);

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_active_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}

///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
void Worker::deactivate() {
  sim->getTraces()->add_trace(worker, id, sched_end);

  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr(); //active priority
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);
}
