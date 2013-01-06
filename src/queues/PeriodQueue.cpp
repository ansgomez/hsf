#include "queues/PeriodQueue.h"

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
PeriodQueue::PeriodQueue() : RunnableQueue() {
  //empty
}

/*********** MEMBER FUNCTIONS ***********/

///This function inserts the new runnable in the queue depending on the Runnable's period
void PeriodQueue::insertRunnable(Runnable *newRunnable) {
  //increase the size counter
  size++;
  
  #if _INFO==1
  cout << "PeriodQueue::insertRunnable - size is now: " << size << endl;
  #endif

  //Base case, the list was empty. The Runnable is now head and tail of queue
  if (head == NULL) {
    head = (Node*) malloc(sizeof(Node));
    head->r = newRunnable;
    tail = head;
    tail->next = NULL;
    #if _DEBUG==1
    cout << "New Head1: " << newRunnable->getId() << endl;
    #endif
    return;
  }

  struct timespec currentPeriod = head->r->getCriteria()->getPeriod();
  struct timespec newPeriod = newRunnable->getCriteria()->getPeriod();

  #if _DEBUG==1
  cout << "Comparing: " << currentPeriod.tv_sec << ":" << currentPeriod.tv_nsec << " vs " << newPeriod.tv_sec << ":" << newPeriod.tv_nsec << endl;
  #endif

  //If in a non-empty queue, newRunnable has a deadline earlier than the head, it becomes the new head
  if(currentPeriod > newPeriod ) {
    //create new node
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->r = newRunnable;
    //link new node to old head
    newNode->next = head;
    //move the head 
    head = newNode;
    #if _DEBUG==1
    cout << "New Head2: " << newRunnable->getId() << endl;
    #endif
    return;
  }

  currentPeriod = tail->r->getCriteria()->getPeriod();

  #if _DEBUG==1
  cout << "Comparing: " << currentPeriod.tv_sec << ":" << currentPeriod.tv_nsec << " vs " << newPeriod.tv_sec << ":" << newPeriod.tv_nsec << endl;
  #endif

  //In a non-empty queue, the new runnable has the latest deadline (later than the tail)
  if( currentPeriod < newPeriod ) {
    //create new node
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->r = newRunnable;
    newNode->next = NULL;
    
    //link the old tail to the new node
    tail->next = newNode;
    //move the tail
    tail = newNode;
    #if _DEBUG==1
    cout << "New tail: " << newRunnable->getId() << endl;
    #endif
    return;
  }

  Node* aux = head->next, *prev=head;

  //This loop will insert the Runnable in any position except first or last
  while(aux != NULL) {
    currentPeriod = aux->r->getCriteria()->getPeriod();

    #if _DEBUG==1
    cout << "Comparing: " << currentPeriod.tv_sec << ":" << currentPeriod.tv_nsec << " vs " << newPeriod.tv_sec << ":" << newPeriod.tv_nsec << endl;
    #endif

    if( currentPeriod > newPeriod) {
      Node *newNode = (Node*) malloc(sizeof(Node));
      newNode->r = newRunnable;
      //insert new node in the middle
      newNode->next = aux;
      prev->next = newNode;
      #if _DEBUG==1
      cout << "New node!" << newRunnable->getId() << endl;
      #endif
      return;
    }
    
    prev=aux;
    aux = aux->next;
  }

  cout << "PeriodQueue::insertRunnable() error! newRunnable was not inserted...\n";
}
