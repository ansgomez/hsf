#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Thread {
 protected:

  unsigned int id;

  int policy;
  pthread_t thread;
  pthread_attr_t thread_attr;
  struct sched_param thread_param;

 public:
  ///Constructor needs nothing to create thread (with inactive priotity);
  Thread(unsigned int _id);

  ///This function is called by the static wrapper. In the Worker object, it should point to fire(), and in the Scheduler object, it should point to schedule()
  virtual void wrapper();

  ///This function returns a pointer to the pthread
  pthread_t* getPthread();

  ///This function return the thread id
  unsigned int getId();

 private:
  ///The runnable thread points to this static function. This function recieves a Runnable object, and calls
  ///the wrapper function in that object
  static void * static_wrapper(void * This);

};
#endif
