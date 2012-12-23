#ifndef _STATISTICS_H
#define _STATISTICS_H

#include "results/Runtime.h"
#include "results/MissedDeadline.h"
#include "results/Trace.h"

#include "util/Enumerations.h"

#include <algorithm>
#include <vector>
#include <semaphore.h>
#include <time.h>

#define N_THREADS   1000
#define N_TRACES    50000
#define N_DEADLINES 1000

using namespace std;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Statistics {
  /*********** VARIABLES ***********/
 private:
  ///This vector holds all of the simulation's runtime statistics
  static vector<Runtime> runtimes;
  ///This vector holds alll of the time traces
  static vector<Trace> traces;
  ///This vector holds all of the missed deadlines
  static vector<MissedDeadline> missedDeadlines;

  ///This variable holds the state. When active, calling the ``add'' functions will add to statistics vector. Otherwise, traces will not be saved
  static int state;

  ///Semaphore to protect writing to the trace vector
  static sem_t trace_sem;

  ///Semaphore to protect writing to the missedDealine vector
  static sem_t deadline_sem;

 public:
  /*********** MEMBER FUNCTIONS ***********/
  ///This function initiates all of the variables
  static void initialize();

  ///This function adds a runtime statistic to the vector
  static void addRuntime(enum _thread_type type, unsigned int t_id, struct timespec ts);

  ///This function adds a trace to the vector
  static void addTrace(enum _thread_type type, unsigned int t_id, enum _task_action act);

  ///This function adds a missed deadline trace to the vector
  static void addMissedDeadline(unsigned int t_id, struct timespec arrival_time);

  ///This function saves all results to a custom csv file
  static void toFile(string filePrefix);

  ///This function enables the collection of statistics
  static void enable();

  ///This function disable the collection of statistics
  static void disable();
  /*
 ///This function helps sort the runtime vector by ID
  struct compareId {
    bool operator()(Runtime* a, Runtime* b) {
      if (a->getId() < b->getId() ) {
	return true;
      }
      else {
	return false;
      }
    }
  };
  */
};
  
#endif
