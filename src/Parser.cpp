#include "Parser.h"

#include "Simulation.h"
#include "Operators.h"
#include "Worker.h"
#include "TDMA.h"
#include "Scheduler.h"
#include "Dispatcher.h"
#include "Runnable.h"
#include "Periodic.h"
#include "BusyWait.h"
#include "Enumerations.h"

#include <iostream>

using namespace std;
using namespace pugi;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

//Constructor needs simulation pointer
Parser::Parser(Simulation *_sim) {
  sim = _sim;
}

//This function converts an XML "time" node to a timespec
struct timespec Parser::parseTime(xml_node n) {
  int time = n.attribute("value").as_int();
  string units = n.attribute("units").value();

  if(units == "sec") {
    return Seconds(time);
  }
  if(units == "ms") {
    return Millis(time);
  }
  if(units == "us") {
    return Micros(time);
  }

  struct timespec x;
  x.tv_sec = -1;
  x.tv_nsec = -1;

  return x;
}

//This function extract information from an XML "worker" and returns its corresponding disp.
Dispatcher* Parser::parseDispatcher(xml_node disp_node, unsigned int *id) {
  string periodicity = disp_node.attribute("periodicity").as_string();
  Dispatcher *disp = NULL;

  *id = *id + 1;

  cout << "Creating Dispatcher " << *id << endl;

  if(periodicity == "periodic") {
    Periodic *p = new Periodic(sim, *id);
    p->setPeriod(parseTime(disp_node.child("period")));
    disp = (Dispatcher*) p;
    sim->addDispatcher(disp);   
  }
  else {
    cout << "Parser Error: Runnable " << *id << "'s periodicity was not recognized\n";
  }

  return disp;
}


///This function receives a Worker node, and it returns the initialized worker object
Worker* Parser::parseWorker(xml_node worker_node, unsigned int *id) {
  string load = worker_node.attribute("load").as_string();
  Worker *worker = NULL;

  if(load == "busy_wait") {
    Dispatcher *d = parseDispatcher(worker_node, id);
    *id = *id + 1;  
    cout << "Creating Worker " << *id << endl;
    BusyWait *bw = new BusyWait(sim, d, parseTime(worker_node.child("wcet")));
    Scheduler *parent;
    worker = new Worker(sim, parent, *id, busy_wait);
    worker->setLoad(bw);
    d->setWorker(worker);
  }
  else {
    cout << "Parser error: Worker " << *id << "'s load was not recognized\n";
  }

  return worker;
}

///This function receives and TDMA node, and parses its load
Scheduler* Parser::parseTDMA(xml_node sched_node, unsigned int *id, int level) {
  cout << "Creating Scheduler " << *id << endl;
  TDMA *sched = new TDMA(sim, *id, level);

  for (xml_node load = sched_node.first_child(); load; load = load.next_sibling()) {
    string type = load.attribute("type").as_string();

    if( type == "worker" ) {
      //*id = *id +1;

#if _DEBUG == 1
      cout << "L" << level << ": Worker - load:" << load.attribute("load").value() << endl;
#endif

      Worker *w = parseWorker(load, id);

      sched->add_load((Runnable*) w);

    }//end of worker
    else if( type == "scheduler" ) {
      string alg = load.attribute("algorithm").as_string();

      cout << "Creating " << alg << " sub-scheduler\n";

      if(alg == "TDMA") {
	*id = *id +1;
#if _DEBUG==1
	cout << "New Sched Runnable {\n";
#endif
	Scheduler *s = parseTDMA(load, id, level+1);
#if _DEBUG==1
	cout << "}\n";
#endif
	sched->add_load((Runnable*) s);
      }//end of tdma
      else {
	cout << "Parser error: '" << type << "' algorithm was not recognized\n";
      }
    }//end of scheduler

    //TODO: only iterate through children of type runnable
    /*
    else {
      cout << "Parser error - Load " << *id << " was not a recognizable Runnable (" << type << ")\n";
    }*/
  }
  
  //TIME SLOTS
  xml_node time_slots = sched_node.child("time_slots");
  for (xml_node slot = time_slots.first_child(); slot; slot = slot.next_sibling()) {
#if _DEBUG == 1
    cout << "L" << level << ": Time slot: " << slot.attribute("value").value() << " " << slot.attribute("units").value() << endl;
#endif
    sched->add_slot(parseTime(slot));
  }

  return (Scheduler*) sched;
}


///This function receives the filepath and initiates the entire simulation
void Parser::parseFile(string filePath) {
  xml_document doc;
  unsigned int id = 1;

  if (!doc.load_file("hsf.xml") ) {
    cout << "Could not find file...\n";
    return;
  }

  xml_node sim_node = doc.child("simulation");

#if _INFO==1
cout << "Simulation name: " << sim_node.attribute("name").value() << endl; 
#endif
  sim->setName(sim_node.attribute("name").value());

#if _INFO==1
cout << "Duration: " << sim_node.child("duration").attribute("value").as_int() << " " << sim_node.child("duration").attribute("units").value() << endl;
#endif
  sim->setDuration(parseTime(sim_node.child("duration")));

  xml_node top_sched = sim_node.child("runnable");

  //cout << "Top Scheduler:" << top_sched.attribute("algorithm").value() << endl;

  //if TDMA
  Scheduler *top = parseTDMA(top_sched, &id, 0);
  sim->setTopScheduler(top);

  cout << "Simulation '" << sim_node.attribute("name").value() << "' has been loaded\n";
}
