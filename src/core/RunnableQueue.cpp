#include "core/RunnableQueue.h"

#include "core/Criteria.h"

#include <iostream>
#include <stdlib.h>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/*********** CONSTRUCTOR ***********/
///Contructor needs nothing
RunnableQueue::RunnableQueue() {
  head = NULL;
  tail = NULL;
  size = 0;
}

/*********** MEMBER FUNCTIONS ***********/

///This function will be implemented by the derived classes
void RunnableQueue::insertRunnable(Runnable *r) {
  cout << "RunnableQueue::insert - this should not print!\n";
}

void RunnableQueue::deleteRunnable(unsigned int id) {
  if(head->r->getId() != id) {
    cout << "RunnableQueue::delete - tried to delete non-active runnable\n";
  }
  else {
    Node *old = head;
    //move the head to the next node
    head = head->next;
    //free the old head
    free(old);
  }
}

///This function reads the head of the queue (destructive read), and returns the pointer to the Runnable
Runnable* RunnableQueue::peek_back() {
  return tail->r;
}

///This function reads the head of the queue (non-destructive read) and returns a pointer to the runnable
Runnable* RunnableQueue::peek_front() {
  return head->r;
}

/*********** GETTER FUNCTIONS ***********/
///This function returns the size of the queue
int RunnableQueue::getSize() {
  return size;
}
