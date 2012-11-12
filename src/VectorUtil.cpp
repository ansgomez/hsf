#ifndef _VECTOR_UTIL_H
#define _VECTOR_UTIL_H

#include "Runnable.h"

#include <vector>

///This function inserts a runnable into a queue ordered by runnable's deadline \invariant{Vec is ordered by Runnable's deadline} \pre{Criteria objects (for all Runnables in vector and r) return a valid deadline
void insertSortedDeadline(vector<Runnable*> &vec, Runnable *r);

#endif
