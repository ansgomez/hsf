#include "Thread.h"

#include "Simulation.h"
#include "Priorities.h"
#include "Statistics.h"

#include <stdlib.h>
#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

///Constructor needs nothing to create thread (with inactive priotity). Note that there should never be a simple Thread object because its wrapper function is null. (It should be called from a subclass)
Thread::Thread(Simulation *s, unsigned int _id) {

  sim = s;
  id = _id;

  cout << "Thread " << _id << " created\n";

  pthread_attr_init(&thread_attr);
  //creating the pthread
  if (pthread_create(&thread,&thread_attr, static_wrapper, this)) {
    cout << "Error creating thread\n";
    exit(-1);
  }
                
  //Setting its priority
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr();
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);	  
}

///Should be overwritten by subclasses
void Thread::wrapper() {
  //empty
  cout << "Thread::wrapper (" << id << ") - This should not print!\n";
}

///This function returns a pointer to the pthread
pthread_t* Thread::getPthread() {
  return &thread;
}

///This function return the thread id
unsigned int Thread::getId() {
  return id;
}

///The runnable thread points to this static function. This function recieves a Thread object, and calls the wrapper function in that object
void * Thread::static_wrapper(void * This)
{
  Thread *t = ((Thread*)This);
  t->wrapper(); 
 
  struct timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
  
  //Save the runtime statistic
  t->sim->getStats()->add_stat(t->type, t->id, ts);
 
  pthread_exit(NULL);
  return NULL;
}
