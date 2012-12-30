#include "queues/FIFOQueue.h"

#include "core/Criteria.h"
#include "core/Runnable.h"

#include <stdlib.h>

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/
 
///Contructor needs nothing
FIFOQueue::FIFOQueue() : RunnableQueue() {
  //empty
}

/*********** MEMBER FUNCTIONS ***********/

///This function inserts the new runnable at the end of the queue.
void FIFOQueue::insertRunnable(Runnable *newRunnable) {
  Node *aux = (Node*) malloc(sizeof(Node));
  aux->r = newRunnable;
  aux->next = NULL;

  //Old tail to new tail
  tail->next = aux;
  //move new tail to the end
  tail = aux;
}
