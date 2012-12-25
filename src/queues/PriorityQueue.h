#ifndef _PRIORITYQUEUE_H
#define _PRIORITYQUEUE_H

#include "core/RunnableQueue.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class PriorityQueue : RunnableQueue {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Contructor needs nothing
  PriorityQueue();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function inserts the new runnable in the queue depending on the Runnable's priority
  void insertRunnable(Runnable *newRunnable);
};

#endif 
