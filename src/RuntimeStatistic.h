#ifndef _RUNTIMESTATISTIC_H
#define _RUNTIMESTATISTIC_H

#include "Enumerations.h"

#include <string>
#include <time.h>

using namespace std;

class RuntimeStatistic {
 private:
  enum _thread_type type;
  unsigned int id;
  struct timespec ts;

 public:
  ///Constructor require all data to build object
  RuntimeStatistic( enum _thread_type _type, unsigned int _id, struct timespec _ts);

  ///This function converts the runtime statistic to a string,
  string toString();

  ///This function returns the statistic's thread id
  unsigned int getId();

  int operator<(RuntimeStatistic b);
};

#endif
