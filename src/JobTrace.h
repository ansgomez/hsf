#ifndef _JOBTRACE_H
#define _JOBTRACE_H

#include "Enumerations.h"

#include <string>
#include <time.h>

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class JobTrace {
  private:
    struct timespec timestamp;

    _thread_type thread_type;

    unsigned int thread_id;

    _task_action task_action;

 public:
    ///This function sets the attributes of the object
    void setTrace(struct timespec ts, _thread_type tt, unsigned int ti, _task_action ta);

    ///This function converts the trace to string for file output
    string toString();
};

#endif
