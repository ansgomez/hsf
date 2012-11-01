#ifndef _CBS_QUEUE_H
#define _CBS_QUEUE_H


class Deadline;

class CBS_Queue {
  private:
    struct cbs_job ready_queue;

    struct cbs_job pending_queue;

    int size;


  public:
    void enqueue(const unsigned int & id, const struct timespec & arrival_time, const Deadline & deadline);

    void dequeue(const unsigned int & id);

};
#endif
