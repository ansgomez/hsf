#include "queues/DeadlineQueue.h"

#include "core/Criteria.h"
#include "util/Operators.h"

#include <stdlib.h>

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Contructor needs nothing
DeadlineQueue::DeadlineQueue() : RunnableQueue() {
  //empty
}

/*********** MEMBER FUNCTIONS ***********/

///This function inserts the new runnable in the queue depending on the Runnable's deadline
void DeadlineQueue::insertRunnable(Runnable *newRunnable) {
  //increase the size counter
  size++;
  //Base case, the list was empty. The Runnable is now head and tail of queue
  if (head == NULL) {
    head = (Node*) malloc(sizeof(Node));
    head->r = newRunnable;
    tail = head;
    return;
  }

  //In a non-empty queue, the new runnable has earliest deadline (earlier than the head)
  if(head->r->getCriteria()->getDeadline() > newRunnable->getCriteria()->getDeadline() ) {
    //create new node
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode->r = newRunnable;

    //link new node to old head
    newNode->next = head;
    //move the head 
    head = newNode;

    return;
  }

  //In a non-empty queue, the new runnable has the latest deadline (later than the tail)
  if(tail->r->getCriteria()->getDeadline() < newRunnable->getCriteria()->getDeadline() ) {
    //create new node
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode->r = newRunnable;
    newNode->next = NULL;
    
    //link the old tail to the new node
    tail->next = newNode;
    //move the tail
    tail = newNode;

    return;
  }

  Node* aux = head;

  //This loop will insert the Runnable in any position except first or last
  while(aux->next != NULL) {
    if(aux->r->getCriteria()->getDeadline() > newRunnable->getCriteria()->getDeadline()) {
      Node *newNode = (Node*) malloc(sizeof(Node));
      newNode->r = newRunnable;
      //insert new node in the middle
      newNode->next = aux->next;
      aux->next = newNode;
      return;
    }
  }
}


