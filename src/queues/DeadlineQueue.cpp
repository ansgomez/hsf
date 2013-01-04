#include "queues/DeadlineQueue.h"

#include "core/Criteria.h"
#include "core/Runnable.h"
#include "util/Operators.h"

#include <stdlib.h>
#include <iostream>

using namespace std;

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

//cout << "DeadlineQueue::insertRunnable - size is now: " << size << endl;

  //Base case, the list was empty. The Runnable is now head and tail of queue
  if (head == NULL) {
    head = (Node*) malloc(sizeof(Node));
    head->r = newRunnable;
    tail = head;
    tail->next = NULL;
    //cout << "New Head1: " << newRunnable->getId() << endl;
    return;
  }

  struct timespec currentDeadline = head->r->getCriteria()->getDeadline();
  struct timespec newDeadline = newRunnable->getCriteria()->getDeadline();

  //cout << "Comparing: " << currentDeadline.tv_sec << ":" << currentDeadline.tv_nsec << " vs " << newDeadline.tv_sec << ":" << newDeadline.tv_nsec << endl;

  //If in a non-empty queue, newRunnable has a deadline earlier than the head, it becomes the new head
  if(currentDeadline > newDeadline ) {
    //create new node
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->r = newRunnable;
    //link new node to old head
    newNode->next = head;
    //move the head 
    head = newNode;
    //cout << "New Head2: " << newRunnable->getId() << endl;
    return;
  }

  currentDeadline = tail->r->getCriteria()->getDeadline();

  //cout << "Comparing: " << currentDeadline.tv_sec << ":" << currentDeadline.tv_nsec << " vs " << newDeadline.tv_sec << ":" << newDeadline.tv_nsec << endl;

  //In a non-empty queue, the new runnable has the latest deadline (later than the tail)
  if( currentDeadline < newDeadline ) {
    //create new node
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->r = newRunnable;
    newNode->next = NULL;
    
    //link the old tail to the new node
    tail->next = newNode;
    //move the tail
    tail = newNode;
    //cout << "New tail: " << newRunnable->getId() << endl;
    return;
  }

  Node* aux = head->next, *prev=head;

  //This loop will insert the Runnable in any position except first or last
  while(aux != NULL) {
    currentDeadline = aux->r->getCriteria()->getDeadline();

    //cout << "Comparing: " << currentDeadline.tv_sec << ":" << currentDeadline.tv_nsec << " vs " << newDeadline.tv_sec << ":" << newDeadline.tv_nsec << endl;

    if( currentDeadline > newDeadline) {
      Node *newNode = (Node*) malloc(sizeof(Node));
      newNode->r = newRunnable;
      //insert new node in the middle
      newNode->next = aux;
      prev->next = newNode;

      //cout << "New node!" << newRunnable->getId() << endl;
      return;
    }
    
    prev=aux;
    aux = aux->next;
  }

  //cout << "DeadlineQueue::insertRunnable() error! newRunnable was not inserted...\n";
}


