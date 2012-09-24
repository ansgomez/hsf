#include "Thread.h"

#include "Priorities.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

///Constructor needs nothing to create thread (with inactive priotity). Note that there should never be a simple Thread object because its wrapper function is null. (It should be called from a subclass)
Thread::Thread() {
  pthread_attr_init(&thread_attr);
  //creating the pthread
  if (pthread_create(&thread,&thread_attr, static_wrapper, this)) {
    printf("Error creating thread\n");
    exit(-1);
  }
                
  //Setting its priority
  pthread_getschedparam(thread, &policy, &thread_param);
  thread_param.sched_priority = Priorities::get_inactive_pr();
  pthread_setschedparam(thread, SCHED_FIFO, &thread_param);	  
}


void Thread::wrapper() {
  //empty
  cout << "Thread::wrapper - This should not print!\n";
}

///The runnable thread points to this static function. This function recieves a Thread object, and calls the wrapper function in that object
void * Thread::static_wrapper(void * This)
{
  ((Thread *)This)->wrapper(); 
  
  return NULL;
}
