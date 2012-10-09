#ifndef _RUNTIMESTATISTIC_H
#define _RUNTIMESTATISTIC_H

#include "Enumerations.h"

#include <string>
#include <time.h>

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class RuntimeStatistic {
  /*********** VARIABLES ***********/
 private:
  ///This variable stores the type of thread the statistic belongs to
  enum _thread_type type;
  ///This variable stores the id of the thread the statistic belongs to
  unsigned int id;
  ///This variable stores the runtime of the thread
  struct timespec ts;

  /*********** CONSTRUCTOR ***********/
 public:
  ///Constructor require all data to build object
  RuntimeStatistic( enum _thread_type _type, unsigned int _id, struct timespec _ts);

  /*********** MEMBER FUNCTIONS ***********/
  ///This function converts the runtime statistic to a string,
  string toString();

  ///This function returns the statistic's thread id
  unsigned int getId();

  ///This operator comparest two runtimestatistics
  int operator<(RuntimeStatistic b);
};

#endif
