#include "queues/RunnableQueue.h"

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
bool RunnableQueue::deleteRunnable(unsigned int id) {
  if(head!=NULL) {
    //If runnable is the head, delete the head
    if(head->r->getId()==id) {
      Node *aux = head;
      head = head->next;
      delete(aux);

      size--;
      return true;
    }
  }
  //if head is NULL, there is nothing to do
  else {
    return false;
  }

  Node *aux = head->next, *prev=head;

  //iterate to find runnable and delete it
  while(aux!=NULL) {
    if(aux->r->getId() == id) {
      prev->next = aux->next;
      delete(aux);
      size--;
      return true ;
    }
    prev = aux;
    aux = aux->next;
  }

  return false;
}

///This function will be implemented by the derived classes. It inserts a runnable into the queue, and returns true if the new runnable is the new head of the queue (used as condition for registering jobs with parent).
bool RunnableQueue::insertRunnable(Runnable *r) {
  cout << "RunnableQueue::insert - this should not print!\n";
  return false;
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
