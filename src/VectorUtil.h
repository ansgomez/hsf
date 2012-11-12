#include "VectorUtil.h"

using namespace std;

///This function inserts a runnable into a queue ordered by runnable's deadline \invariant{Vec is ordered by Runnable's deadline} \pre{Criteria objects (for all Runnables in vector and r) return a valid deadline

void insertSortedDeadline(vector<Runnable*> &vec, Runnable *r) {
  if(vec.size() == 0) {
    vec.push_back(r);
    return;
  }

  for(int c=0;c<vec.size();c++) {
    if(vec[c]->getCriteria()->getDeadline() > r->getCriteria()->getDeadline()) {
      vector<int>::iterator it = vec.begin() + c;
      it = vec.insert(it, n);
      return;
    }
  }

  vec.push_back(n);
}
