#include "results/Statistics.h"

#include "util/TimeUtil.h"

#include <fstream>
#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** VARIABLES ***********/

///This vector holds all of the simulation's runtime statistics
vector<Runtime> Statistics::runtimes;

///This vector holds alll of the time traces
vector<Trace> Statistics::traces;

///This vector holds all of the missed deadlines
vector<MissedDeadline> Statistics::missedDeadlines;

//This vector holds the thread id's of all workers
vector<unsigned int> Statistics::workerId;

///This variable holds the state. If state==1, calling the ``add'' functions will add to statistics vectors. Otherwise, traces will not be saved
int Statistics::state = 0;

///Semaphore to protect writing to the trace vector
sem_t Statistics::trace_sem;

///Semaphore to protect writing to the missedDealine vector
sem_t Statistics::deadline_sem;

/*********** MEMBER FUNCTIONS ***********/

///This function adds a missed deadline trace to the vector
void Statistics::addMissedDeadline(unsigned int t_id, struct timespec arrival_time, struct timespec deadline) {
  if(state == 1) {
    MissedDeadline md;
    struct timespec aux;

    aux = TimeUtil::getTime(relative);

    sem_wait(&deadline_sem);
      md.setMissedDeadline(t_id, arrival_time, deadline,  aux);
      missedDeadlines.push_back(md);
    sem_post(&deadline_sem);
  }
}

///This function adds a runtime to the vector
void Statistics::addRuntime(enum _thread_type type, unsigned int id, struct timespec _rt) {
  Runtime rt;

  rt.setRuntime(type, id, _rt);
  runtimes.push_back(rt);
}

///This function adds a trace to the vector
void Statistics::addTrace(enum _thread_type type, unsigned int t_id, enum _task_action act) {

  if(state == 1) {
    Trace t;
    struct timespec aux;

    aux = TimeUtil::getTime(relative);
    sem_wait(&trace_sem);
      t.setTrace(aux, type, t_id, act);
      traces.push_back(t);
    sem_post(&trace_sem);
  }
}

///This function should be called by the Worker constructor to 'register' its id
void Statistics::addWorkerId(unsigned int id) {
#if _INFO ==1
  cout << "Adding Worker ID: " << id << endl;
#endif 

  workerId.push_back(id);
}

///This function enables the collection of statistics
void Statistics::enable() {
  state = 1;
}

///This function disable the collection of statistics
void Statistics::disable() {
  state = 0;
}

///This function initiates all of the variables
void Statistics::initialize() {
  runtimes.reserve(N_THREADS);
  traces.reserve(N_TRACES);
  missedDeadlines.reserve(N_DEADLINES);

  //Initialize semaphores to 1, so it acts like a mutex
  sem_init(&trace_sem, 0, 1);
  sem_init(&deadline_sem, 0, 1);

  //must call enable() before registering statistics
  state = 0;
}

///This function saves to custom csv file
void Statistics::toFile(string filePrefix) {  
  ofstream file;

  /************ SAVING MISSED DEADLINES *********/
  MissedDeadline aux_m;
  file.open((filePrefix+"_missed_deadlines.csv").data());
  for(unsigned int c=0;c<missedDeadlines.size();c++) {
    aux_m = missedDeadlines[c];
    file << aux_m.toString() << endl; 
  }
  file.close();

  /************ SAVING RUNTIMES *********/
  Runtime aux_r;

  //First sort the vector, then save to file
  sort(runtimes.begin(), runtimes.end(), compareId());
  file.open((filePrefix+"_runtimes.csv").data());
  for(unsigned int c=0;c<runtimes.size();c++) {
    aux_r = runtimes[c];
    file << aux_r.toString() << endl; 
  }
  file.close();

  /************ SAVING TRACES *********/
  Trace aux_t;
  file.open((filePrefix+"_traces.csv").data());
  for(unsigned int c=0;c<traces.size();c++) {
    aux_t = traces[c];
    file << aux_t.toString() << "\n"; 
  }
  file.close();

  //Change the owner and permissions of generated files
  //system(("chown hsf:hsf " + filePrefix + "_*.csv").data() );
  system(("chmod 666 " + filePrefix + "_*.csv").data() );
}
