#include "queues/PeriodQueue.h"

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

 /*********** CONSTRUCTOR ***********/
 
///Contructor needs nothing
PeriodQueue::PeriodQueue() : RunnableQueue() {
  //empty
}

/*********** MEMBER FUNCTIONS ***********/

///This function inserts the new runnable in the queue in such a ways that no job before it (in the queue) has a lower period. If only this method is used to access the queue, then the queue will always be sorted by ascending period
void PeriodQueue::insert(Runnable *newRunnable) {
  //Base case, the list was empty. The Runnable is now head and tail of queue
  if (head == NULL) {
    head = (Node*) malloc(sizeof(Node))
    head->r = newRunnable;
    tail = head;
    return;
  }

  //In a non-empty queue, the new runnable has earliest deadline (earlier than the head)
  if(head->r->getCriteria()->getPeriod() > newRunnable->getCriteria()->getPeriod() ) {
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
  if(tail->r->getCriteria()->getPeriod() < newRunnable->getCriteria()->getPeriod() ) {
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
    if(aux->r->getCriteria()->getPeriod() > newRunnable()->getCriteria()->getPeriod()) {
      Node *newNode = (Node*) malloc(sizeof(Node));
      newNode->r = newRunnable;
      //insert new node in the middle
      newNode->next = aux->next;
      aux->next = newNode;
      return;
    }
  }
}
