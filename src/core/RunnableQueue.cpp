#include "core/RunnableQueue.h"

#include "core/Criteria.h"
#include "core/Runnable.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/
///Contructor needs nothing
RunnableQueue::RunnableQueue() {
  head = NULL;
  tail = NULL;
  size = 0;
}

/*********** MEMBER FUNCTIONS ***********/

///This function deletes the queue's pointer to the Runnable with the specified ifndef
void RunnableQueue::deleteRunnable(unsigned int id) {
  if(head==NULL) {
    cout << "RunnableQueue::deleteRunnable - head is null!\n";
    return;
  }

  if(head->r->getId() != id) {
    cout << "RunnableQueue::deleteRunnable - tried to delete non-active runnable\n";
  }
  else {
    Node *old = head;
    //move the head to the next node
    head = head->next;
    //free the old head
    free(old);
    size--;
  }
}

///This function will be implemented by the derived classes
void RunnableQueue::insertRunnable(Runnable *r) {
  cout << "RunnableQueue::insert - this should not print!\n";
}

///This function reads the head of the queue (destructive read), and returns the pointer to the Runnable
Runnable* RunnableQueue::peek_back() {
  if(tail==NULL)
    return NULL;

  return tail->r;
}

///This function reads the head of the queue (non-destructive read) and returns a pointer to the runnable
Runnable* RunnableQueue::peek_front() {
  if(head==NULL)
    return NULL;

  return head->r;
}

///This function reads the head of the queue, erases it from the RunnableQueue, and returns the head pointer
Runnable* RunnableQueue::pop_front() {
  if(head==NULL)
    return NULL;

  Runnable* aux = head->r;

  deleteRunnable(aux->getId());

  return aux;
}

/*********** GETTER FUNCTIONS ***********/

///This function returns the size of the queue
int RunnableQueue::getSize() {
  return size;
}
