#include "Thread.h"
#include "defines.h"

#include "Priorities.h"

#include <stdlib.h>
#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

///Constructor needs nothing to create thread (with inactive priotity). Note that there should never be a simple Thread object because its wrapper function is null. (It should be called from a subclass)
Thread::Thread(unsigned int _id) {

  id = _id;

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
  cout << "Thread::wrapper - This should not print!\n";
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
  ((Thread *)This)->wrapper(); 
 
  struct timespec t2;
  clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t2);
  cout << "Thread " << ((Thread *)This)->id << " ran for " << t2.tv_sec*1000000+t2.tv_nsec/1000 << " micros\n";
 
  pthread_exit(NULL);
  return NULL;
}
