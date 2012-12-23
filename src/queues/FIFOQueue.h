#ifndef _FIFOQUEUE_H
#define _FIFOQUEUE_H

#include "core/RunnableQueue.h"

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class FIFOQueue : RunnableQueue {
 public:
  /*********** CONSTRUCTOR ***********/
  ///Contructor needs nothing
  FIFOQueue();

  /*********** MEMBER FUNCTIONS ***********/
  ///This function inserts the new runnable in the queue depending on the derived class' criteria
  void insert(Runnable *newRunnable);

  ///This function reads the head of the queue and returns a pointer to the runnable
  Runnable* peek_front();

  ///This function reads the tail of the queue and returns the pointer to the Runnable
  Runnable* peek_back();

  ///This function deletes the queue's pointer to the Runnable with the specified ifndef
  void delete(unsigned int id);
};

#endif