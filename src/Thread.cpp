#include "Thread.h"

#include "Simulation.h"
#include "Priorities.h"
#include "Statistics.h"

#include <stdlib.h>
#include <iostream>

#define _INFO 0

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/********************* CONSTRUCTOR *********************/

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

/********************* MEMBER FUNCTIONS *********************/

///Should be overwritten by subclasses
void Thread::wrapper() {
  //empty
  cout << "Thread::wrapper (" << id << ") - This should not print!\n";
}

///This function blocks the calling thread until this thread exits
void Thread::join() {

#if _INFO == 1
  cout << "Attempting to joining thread " << id << endl;
#endif

  /*
  if(thread==NULL) 
    cout << "Thread::Join - NULL PROBLEM\n";
  */

  pthread_join(thread, NULL);

#if _INFO == 1
  cout << "Successfully joined thread " << id << endl;
#endif
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

  /*
  if(t==NULL) {
    cout << "Thread::Static_Wrapper - DEFINATELY A PROBLEM\n";
  }
  */

#if _INFO==1
  cout << "Thread " << t->getId() << " has exited wrapper\n";
#endif

  struct timespec ts;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
 
  //Save the runtime statistic
  t->sim->getStats()->add_stat(t->type, t->id, ts);

  pthread_exit(NULL);
  return NULL;
}
