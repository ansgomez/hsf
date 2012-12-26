#ifndef _RUNNABLE_H
#define _RUNNABLE_H

#include "pthread/Thread.h"
#include "util/Enumerations.h"

class Intermediary;
class Task;
class Criteria;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Runnable : public Thread {

 protected:

  /*********** VARIABLES ***********/

  ///Runnable's schedulable criteria (this is the criteria used to schedule it)
  Criteria* criteria;
  
  ///Pointer to Task currently being 'executed' by the runnable
  Task* current_task;

  ///Pointer to the scheduler that this runnable belongs to
  Intermediary* parent;

  ///Auxiliary variable to hold the state of runnable
  _runnable_state state;

 public:

  /*********** CONSTRUCTOR ***********/

  ///Constructor needs simulation pointer and an id
  Runnable(unsigned int _id);

  /*********** INHERITED FUNCTIONS ***********/
  
  /**** FROM THREAD ****/

  ///This function joins the calling thread with the object's pthread
  void join();

  ///This function will be defined by subclasses
  virtual void wrapper();

  /**** FROM RUNNABLE ****/
  
  ///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
  virtual void activate();

  ///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
  virtual void deactivate();


  /*********** GETTERS AND SETTERS ***********/
  ///This function return the runnable's schedulable criteria
  Criteria* getCriteria();
  ///This function sets the runnable's parent
  void setParent(Intermediary* p);
};
#endif
