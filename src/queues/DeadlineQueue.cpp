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

  //If in a non-empty queue, newRunnable has a deadline earlier than the head, it becomes the new head
  if(head->r->getCriteria()->getDeadline() > newRunnable->getCriteria()->getDeadline() ) {
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

  //In a non-empty queue, the new runnable has the latest deadline (later than the tail)
  if(tail->r->getCriteria()->getDeadline() < newRunnable->getCriteria()->getDeadline() ) {
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

  Node* aux = head;

  //This loop will insert the Runnable in any position except first or last
  while(aux != NULL) {
    if(aux->r->getCriteria()->getDeadline() > newRunnable->getCriteria()->getDeadline()) {
      Node *newNode = (Node*) malloc(sizeof(Node));
      newNode->r = newRunnable;
      //insert new node in the middle
      newNode->next = aux->next;
      aux->next = newNode;
      //cout << "New node!" << newRunnable->getId() << endl;
      return;
    }

    aux = aux->next;
  }

  //cout << "DeadlineQueue::insertRunnable() error! newRunnable was not inserted...\n";
}


