#include "util/Parser.h"

#include "core/Dispatcher.h"
#include "core/ResourceAllocator.h"
#include "core/Runnable.h"
#include "core/Scheduler.h"
#include "core/Simulation.h"
#include "core/Worker.h"
#include "dispatchers/Periodic.h"
#include "schedulers/TDMA.h"
#include "tasks/BusyWait.h"
#include "util/Enumerations.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

#define _INFO 0
#define _DEBUG 0

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

//Constructor needs simulation pointer
Parser::Parser(Simulation *_sim) {
  sim = _sim;
}

/*********** PUBLIC MEMBER FUNCTIONS ***********/

///This function receives the filepath and initiates the entire simulation
void Parser::parseFile(string filePath) {
  xml_document doc;
  unsigned int id = 1;

  //Load file
  if (!doc.load_file(filePath.data()) ) {
    cout << "Could not find file...\n";
    return;
  }

  //Create parent xml_node
  xml_node sim_node = doc.child("simulation");

  //Set parent (simulation) attributes
  sim->setName(sim_node.attribute("name").value());
  sim->setDuration(parseTime(sim_node.child("duration")));

  //Create top_sched node
  xml_node top_sched = sim_node.child("runnable");

  //if TDMA
  Scheduler *top = (Scheduler*) parseTDMA(top_sched, &id, 0);
  sim->setTopScheduler(top);

  cout << "... '" << sim_node.attribute("name").value() << "' has been loaded\n";
}

/*********** PRIVATE MEMBER FUNCTIONS ***********/

//This function extract information from an XML "worker" and returns its corresponding disp.
Dispatcher* Parser::parseDispatcher(xml_node disp_node, unsigned int *id) {
  string periodicity = disp_node.attribute("periodicity").as_string();
  Dispatcher *disp = NULL;

  *id = *id + 1;

#if _INFO==1
  cout << "Creating Dispatcher " << *id << endl;
#endif

  if(periodicity == "periodic") {
    Periodic *p = new Periodic(*id);
    p->setPeriod(parseTime(disp_node.child("period")));
    disp = (Dispatcher*) p;
    sim->addDispatcher(disp);   
  }
  else {
    cout << "Parser Error: Runnable " << *id << "'s periodicity was not recognized\n";
  }

  return disp;
}

///This function receives and TDMA node, and parses its load
TDMA* Parser::parseTDMA(xml_node sched_node, unsigned int *id, int level) {

  TDMA *sched = new TDMA(*id, level);

  //iterate through all of the children nodes
  for (xml_node load = sched_node.first_child(); load; load = load.next_sibling()) {
    string type = load.attribute("type").as_string();

    //If child is worker, parse a worker
    if( type == "worker" ) {
      Worker *w = parseWorker((ResourceAllocator*)sched, load, id);

      sched->add_load((Runnable*) w);

    }
    //If child is scheduler, parse the correct scheduler
    else if( type == "scheduler" ) {
      string alg = load.attribute("algorithm").as_string();

      if(alg == "TDMA") {
	*id = *id +1;

	TDMA* s = (TDMA*) parseTDMA(load, id, level+1);

	sched->add_load((Runnable*) s);
      }//end of tdma
      else {
	cout << "Parser error: '" << type << "' algorithm was not recognized\n";
      }
    }//end of scheduler

    //TODO: only iterate through children of type runnable
  }
  
  //TIME SLOTS
  xml_node time_slots = sched_node.child("time_slots");
  for (xml_node slot = time_slots.first_child(); slot; slot = slot.next_sibling()) {
    sched->add_slot(parseTime(slot));
  }

  return sched;
}

//This function converts an XML "time" node to a timespec
struct timespec Parser::parseTime(xml_node n) {
  int time = n.attribute("value").as_int();
  string units = n.attribute("units").value();
  struct timespec ret;

  if(units == "sec") {
    ret = TimeUtil::Seconds(time);
  }
  else if(units == "ms") {
    ret = TimeUtil::Millis(time);
  }
  else if(units == "us") {
    ret = TimeUtil::Micros(time);
  }
  else {
    cout << "Parser error: could not recognize time unit!\n";
  }

  return ret;
}


///This function receives a Worker node, its parent, and it returns the initialized worker object
Worker* Parser::parseWorker(ResourceAllocator* parent, xml_node worker_node, unsigned int *id) {
  string load = worker_node.attribute("load").as_string();
  Worker *worker = NULL;

  if(load == "busy_wait") {
    Dispatcher *d = parseDispatcher(worker_node, id);
    *id = *id + 1;  
#if _INFO==1
    cout << "Creating Worker " << *id << endl;
#endif
    BusyWait *bw = new BusyWait(d, parseTime(worker_node.child("wcet")));

    worker = new Worker(parent, *id, busy_wait);
    worker->setLoad(bw);
    d->setWorker(worker);
  }
  else {
    cout << "Parser error: Worker " << *id << "'s load was not recognized\n";
  }

  return worker;
}
