#ifndef _PERIODQUEUE_H
#define _PERIODQUEUE_H

#include "queues/RunnableQueue.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class PeriodQueue : RunnableQueue {

 public:

  /*********** CONSTRUCTOR ***********/

  ///Contructor needs nothing
  PeriodQueue();

  /*********** MEMBER FUNCTIONS ***********/

  ///This function inserts the new runnable in the queue depending on the Runnable's period
  void insertRunnable(Runnable *newRunnable);
};

#endif 
