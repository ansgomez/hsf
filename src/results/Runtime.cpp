#include "Runtime.h"

#include "util/TimeUtil.h"

/********************* MEMBER FUNCTIONS *********************/

///This sets the thread type, id, and runtime of trace
  void setRuntime(enum _thread_type _type, unsigned int _id, struct timespec _ts) {
  type = _type;
  id = _id;
  ts = _ts;
}

///This function converts the runtime statistic to a string,
string Runtime::toString() {
  stringstream out;

  out << type << "," << id << "," << TimeUtil::convert_us(ts);

  return out.str();
}

///This function returns the statistic's thread id
unsigned int Runtime::getId() {
  return id;
}

///This operator defines the sorting criteria to the the statistic's id
int Runtime::operator<(RuntimeStatistic b) {
  return this->getId() < b.getId();
}
