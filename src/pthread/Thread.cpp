#include "pthread/Thread.h"

#include "core/Simulation.h"
#include "pthread/Priorities.h"
#include "results/Statistics.h"

#include <iostream>
#include <stdlib.h>

#define _INFO 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs nothing to create thread (with inactive priotity). Note that there should never be a simple Thread object because its wrapper function is null. (It should be called from a subclass)
Thread::Thread(Simulation *s, unsigned int _id) {

  sim = s;
  id = _id;

#if _INFO==1
  cout << "Thread " << _id << " created\n";
#endif

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

Thread::~Thread() {
  cout << "Thread Object " << id << " is being destructed\n";
}

/*********** MEMBER FUNCTIONS ***********/

///This function blocks the calling thread until this thread exits
void Thread::join() {
  pthread_join(thread, NULL);
}

///The runnable thread points to this static function. This function recieves a Thread object, and calls the wrapper function in that object
void * Thread::static_wrapper(void * This)
{
  Thread *t = ((Thread*)This);
  t->wrapper(); 

  struct timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
 
  //Save the runtime statistic
  Statistics::addRuntime(t->thread_type, t->id, ts);

  pthread_exit(NULL);
  return NULL;
}

///Should be overwritten by all subclasses
void Thread::wrapper() {
  //empty
  cout << "Thread::wrapper (" << id << ") - This should not print!\n";
}

/*********** GETTER AND SETTER FUNCTIONS ***********/


///This function return the thread id
unsigned int Thread::getId() {
  return id;
}

///This function sets the priority of the thread
void Thread::setPriority(int priority) {
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = priority;
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);  
}
