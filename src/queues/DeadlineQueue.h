#ifndef _DEADLINEQUEUE_H
#define _DEADLINEQUEUE_H

#include "queues/RunnableQueue.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class DeadlineQueue : RunnableQueue {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Contructor needs nothing
  DeadlineQueue();

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM RUNNABLEQUEUE ****/   
   
  ///This function inserts the new runnable in the queue depending on the Runnable's deadline
  void insertRunnable(Runnable *newRunnable);
};

#endif 
