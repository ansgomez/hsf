#ifndef _THREAD_H
#define _THREAD_H

#include "util/Enumerations.h"

#include <pthread.h>

using namespace std;

class Simulation;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Thread {
  /*********** VARIABLES ***********/
 protected:
  ///Pointer to the simulation
  Simulation* sim;

  ///Thread id - a unique value among all threads in a simulation
  unsigned int id;

  ///Type indicates wether the thread is a scheduler, worker, dispatcher, server, or idle
  _thread_type thread_type;

  ///This is an auxiliary variable to set the thread's priority
  int policy;

  ///The actual pthread variable
  pthread_t thread;

  ///Auxiliary variable to set the thread's attributes
  pthread_attr_t thread_attr;

  ///Auxiliary variable to the set the thread's parameters
  struct sched_param thread_param;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor needs nothing to create thread (with inactive priotity);
  Thread(Simulation *s, unsigned int _id);
  ~Thread();

  /*********** MEMBER FUNCTIONS ***********/
  ///This function sets the priority of the thread
  void setPriority(int);

  ///This function is called by the static wrapper. In the Worker object, it should point to fire(), and in the Scheduler object, it should point to schedule()
  virtual void wrapper();

  ///Calling this function will block the calling thread until this thread exits
  virtual void join();

 private:
  ///The runnable thread points to this static function. This function recieves a Runnable object, and calls
  ///the wrapper function in that object
  static void * static_wrapper(void * This);

  /*********** GETTER FUNCTIONS ***********/
 public:
  ///This function returns the thread id
  unsigned int getId();

};
#endif
