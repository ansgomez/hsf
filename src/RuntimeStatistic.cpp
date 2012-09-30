#include "RuntimeStatistic.h"

#include "TimeUtil.h"

#include <time.h>
#include <string>
#include <sstream>

using namespace std;

///Constructor require all data to build object
RuntimeStatistic::RuntimeStatistic( enum _thread_type _type, unsigned int _id, struct timespec _ts) {
  type = _type;
  id = _id;
  ts = _ts;
}

///This function converts the runtime statistic to a string,
string RuntimeStatistic::toString() {
  stringstream out;

  out << type << "," << id << "," << TimeUtil::convert_us(ts);

  return out.str();
}


///This function returns the statistic's thread id
unsigned int RuntimeStatistic::getId() {
  return id;
}

int RuntimeStatistic::operator<(RuntimeStatistic b) {
  return this->getId() < b.getId();
}
