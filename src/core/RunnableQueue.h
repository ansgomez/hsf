#ifndef _RUNNABLEQUEUE_H
#define _RUNNABLEQUEUE_H

#include "core/Runnable.h"

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

   ///This function inserts the new runnable in the queue in such a way that no job before it (in the queue) has a lower (numerical) priority than itself. If only this method is used to access the queue, then the queue will always be sorted by descending priority
  
  ///This function inserts the new runnable in the queue in such a ways that no job before it (in the queue) has a lower period. If only this method is used to access the queue, then the queue will always be sorted by ascending period

typedef struct _Node {
  Runnable *r;
  struct _Node *next;
} Node;

class RunnableQueue {
 protected:
  ///This variable holds the size of the queue
  int size;

  ///The head of the queue points the current job. The rest of the queue is ordered depending of the method used to insert into the queue
  Node* head;

  ///The tail of the queue points to the last job on the active queue.
  Node* tail;
  
 public:
  /*********** CONSTRUCTOR ***********/
  ///Contructor needs nothing
  RunnableQueue();

  /*********** MEMBER FUNCTIONS ***********/
  ///This function inserts the new runnable in the queue depending on the derived class' criteria
  virtual void insertRunnable(Runnable *newRunnable);

  ///This function reads the head of the queue and returns a pointer to the runnable
  Runnable* peek_front();

  ///This function reads the tail of the queue and returns the pointer to the Runnable
  Runnable* peek_back();

  ///This function deletes the queue's pointer to the Runnable with the specified ifndef
  void deleteRunnable(unsigned int id);

  /*********** GETTER FUNCTIONS ***********/
  ///This function returns the size of the RunnableQueue
  int getSize();
};

#endif 
