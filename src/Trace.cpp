#include "Trace.h"

#include "Enumerations.h"
#include "Simulation.h"
#include "TimeUtil.h"
#include "Operators.h"
#include "lib/mathgl/SimulationFigure.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <vector>

using namespace std;

#define MAX_TRACES 1000

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

//Constructor requires a pointer to the simulation that the traces belong to
Trace::Trace(Simulation *s) {
  sim = s;

  trace_full = 0;

  traces.reserve(MAX_TRACES);//preallocate memory

  //Initialize semaphore to 1, so it acts like a mutex
  sem_init(&trace_sem, 0, 1);
}

///This function adds a trace to the vector
void Trace::add_trace(enum _thread_type type, unsigned int t_id, enum _task_action act) {
#if _DEBUG == 1
  cout << "Adding a new trace\n";
#endif

  if(sim->isSimulating() == 1) {
    if(traces.size() < MAX_TRACES) {
      JobTrace jt;
      struct timespec aux;

      sem_wait(&trace_sem);
      {
	//clock_gettime(CLOCK_REALTIME, &aux);
	aux = TimeUtil::getTime(relative);
	jt.setTrace(aux, type, t_id, act);
	
	traces.push_back(jt);
      }
      sem_post(&trace_sem);
    }
    else { //Trace array is full
      trace_full=1;
    }
  }
}

///This function saves to custom csv file
void Trace::to_file() {
  ofstream file;
  JobTrace *aux;

#if _INFO == 1
  cout << "Saving to file...\n";
#endif

  file.open((sim->getName()+".csv").data());

  for(unsigned int c=0;c<traces.size();c++) {

    aux = &traces[c];

    if(aux->getTimestamp().tv_sec != 0 || aux->getTimestamp().tv_nsec != 0) {
      file << aux->toString() << "\n"; 
    }
  }

  file.close();
}

///This function generates a figure from traces
void Trace::to_figure() {
  char label[30];
  char title[20];
  unsigned int c;
  SimulationFigure *sf = new SimulationFigure;
  vector<unsigned int> *worker_id = sim->getWorker_id();
  vector<unsigned int>::iterator it;
  unsigned int id;

  unsigned int n_threads = worker_id->size();

  unsigned int *start_sched_us = (unsigned int*) malloc(n_threads*sizeof(unsigned int));
  unsigned int *start_time =  (unsigned int*) malloc(n_threads*sizeof(unsigned int));

  struct timespec range = Millis(100);
  
  //Define values
  sf->setNPlots(n_threads);//sim->getNWorkers());
  sf->setTimeRange(TimeUtil::convert_ms(range));//trace_time_range_ms);

  //Initialize each worker plot
  for(c=0;c<n_threads;c++) {
    *(start_time+c*sizeof(unsigned int)) = 0;
    *(start_sched_us+c*sizeof(unsigned int)) = 0;
    sprintf(title, "Worker %d ", c+1);
    sf->initPlot(c, title);
  }

  //Iterate through traces and build job trace
  for(c=0; c<traces.size() ;c++) {

    it = find(worker_id->begin(), worker_id->end(), traces[c].getId());
    id = it-worker_id->begin();

    if(traces[c].getTimestamp() > range)
      break;

    switch(traces[c].getAction()) {
    case task_arrival:
      /*
      if( == aperiodic) 
	{
	sprintf(label, "\\big{C = %lu}", t_inf[traces[c].thread_id].wcet);
      }
      else {
	strcpy(label, "\0");
	}*/
      strcpy(label, "Job\0");
      sf->drawArrival(id, TimeUtil::convert_ms(traces[c].getTimestamp()), label);
      break;

    case task_start:
      *(start_time+id*sizeof(unsigned int)) = TimeUtil::convert_us(traces[c].getTimestamp());
      break;

    case sched_start:
      *(start_sched_us+id*sizeof(unsigned int)) = TimeUtil::convert_us(traces[c].getTimestamp());
      if(*(start_time+id*sizeof(unsigned int)) != 0) 
	*(start_time+id*sizeof(unsigned int)) = *(start_sched_us+id*sizeof(unsigned int));
      break;

    case sched_end:
      if(true) { //jobtrace_sched_en == 1){
	float x = (*(start_sched_us+id*sizeof(unsigned int)))/1000;
	sf->drawSched(id, x,TimeUtil::convert_ms(traces[c].getTimestamp()) );
      }

      if(*(start_time+id*sizeof(unsigned int)) != 0) {
	float x = (*(start_time+id*sizeof(unsigned int)))/1000;
	sf->drawJob(id, x, TimeUtil::convert_ms(traces[c].getTimestamp()) );
      }

      break;

    case task_end:
      float x = (*(start_time+id*sizeof(unsigned int)))/1000;
      sf->drawJob(id, x, TimeUtil::convert_ms(traces[c].getTimestamp()) );
      *(start_time+id*sizeof(unsigned int)) = 0;
      break;
    }
  }

  sf->exportPNG();
}
