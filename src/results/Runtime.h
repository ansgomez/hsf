#ifndef _RUNTIME_H
#define _RUNTIME_H

#include "util/Enumerations.h"

#include <time.h>
#include <string>

using namespace std;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Runtime {

 private:

  /*********** VARIABLES ***********/

  ///This variable stores the type of thread the statistic belongs to
  enum _thread_type type;
  ///This variable stores the id of the thread the statistic belongs to
  unsigned int id;
  ///This variable stores the runtime of the thread
  struct timespec ts;

 public:

  /*********** MEMBER FUNCTIONS ***********/

  ///This sets the thread type, id, and runtime of trace
  void setRuntime(enum _thread_type _type, unsigned int _id, struct timespec _ts);

  ///This function converts the runtime statistic to a string,
  string toString();

  ///This function returns the statistic's thread id
  unsigned int getId();

  ///This operator comparest two runtimestatistics
  int operator<(Runtime b);
};

#endif
