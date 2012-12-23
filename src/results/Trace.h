#ifndef _TRACE_H
#define _TRACE_H

#include "util/util/Enumerations.h"

#include <vector>
#include <string>

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Trace {
/*********** VARIABLES ***********/
 private:
  ///This variable will store the trace's timestamp
  struct timespec timestamp;

  ///This variable stores the type of thread the trace belongs to
  _thread_type thread_type;

  ///This variable store the id of the thread the trace belongs to
  unsigned int thread_id;

  ///This variable stores the action that is being traced
  _task_action task_action;

  /*********** MEMBER FUNCTIONS ***********/
 public:
  ///This function sets the attributes of the object
  void setTrace(struct timespec ts, _thread_type tt, unsigned int ti, _task_action ta);

  ///This function converts the trace to string for file output
  string toString();

  /*********** GETTER FUNCTIONS ***********/
  ///This function returns the thread type
  _thread_type getType();

  ///This function returns the timestamp
  struct timespec getTimestamp();

  ///This function returns the thread ID
  unsigned int getId();

  ///This function return the traced action
  _task_action getAction();
};


#endif
