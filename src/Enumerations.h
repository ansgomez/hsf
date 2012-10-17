#ifndef _ENUMERATIONS_H
#define _ENUMERATIONS_H

///This enum describes what thread types can produce traces
enum _thread_type {
  idle,
  scheduler,
  dispatcher,
  server,
  worker
};

///This enum describes all the actions that can be traced
enum _task_action {
  task_arrival,
  sched_start,
  sched_end,
  task_start,
  task_end
};

///This enumeration describes the different scheduler types
enum _sched_type {
  tdma,
  cbs,
  edf,
  dbs,
  rm
};

///This enumeration descibes the possible task models
enum _task_periodicity {
  periodic,
  aperiodic,
  periodic_jitter
};

///This enumeration describes the possible loads for a worker
enum _task_load {
  busy_wait,
  video

};

///This enumeration describes the two time types
enum _time_type {
  absolute,
  relative
};


///This enumeration describs the two runnable states
enum _runnable_state {
  activated,
  deactivated
};

#endif
