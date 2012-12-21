#ifndef _CBS_QUEUE_H
#define _CBS_QUEUE_H

class Deadline;
class CBS;
class Task;

struct cbs_job {
  CBS *server;
  Task *task;
};

class CBS_Queue {
  private:
    struct cbs_job ready_queue;

    struct cbs_job pending_queue;

    int size;


  public:
    void enqueue(unsigned int id, struct timespec arrival_time);

    void dequeue(unsigned int id);
};
#endif
