#ifndef _FIFOQUEUE_H
#define _FIFOQUEUE_H

#include "core/RunnableQueue.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class FIFOQueue : RunnableQueue {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Contructor needs nothing
  FIFOQueue();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function inserts the new runnable in the queue depending on Runnable's arrivalTime
  void insertRunnable(Runnable *newRunnable);
};

#endif
