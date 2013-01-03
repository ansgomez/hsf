#include "util/Parser.h"

#include "core/Dispatcher.h"
#include "core/ResourceAllocator.h"
#include "core/Runnable.h"
#include "core/Scheduler.h"
#include "core/Simulation.h"
#include "core/Worker.h"
#include "dispatchers/Periodic.h"
#include "dispatchers/PeriodicJitter.h"
#include "schedulers/EDF.h"
#include "schedulers/TDMA.h"
#include "tasks/BusyWait.h"
#include "tasks/Video.h"
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

  Scheduler *top = parseScheduler(NULL, top_sched, &id, 0); 
  sim->setTopScheduler(top);

  cout << "\n***   Loaded '" << sim_node.attribute("name").value() << "'\t***\n";
}

/*********** PRIVATE MEMBER FUNCTIONS ***********/

//This function extract information from an XML "worker" and returns its corresponding disp.
Dispatcher* Parser::parseDispatcher(xml_node disp_node, unsigned int *id) {
  string periodicity = disp_node.attribute("periodicity").as_string();
  Dispatcher *disp = NULL;

  #if _INFO==1
  cout << "Creating Dispatcher " << *id << endl;
  #endif

  if(periodicity == "periodic") {
    Periodic *p = new Periodic(*id);
    p->setPeriod(parseTime(disp_node.child("period")));
    disp = (Dispatcher*) p;
  }
  else if(periodicity == "periodic_jitter") {
    PeriodicJitter *p = new PeriodicJitter(*id);
    p->setPeriod(parseTime(disp_node.child("period")));
    p->setJitter(parseTime(disp_node.child("jitter")));
    disp = (Dispatcher*) p;
  }
  else {
    cout << "Parser Error: Runnable " << *id << "'s periodicity was not recognized\n";
  }

  if(disp!=NULL) {
    sim->addDispatcher(disp); 
    sim->addThread((Thread*)disp);
  }

  return disp;
}

///This function receives an EDF node and it parses the entire node to return the full object
EDF* Parser::parseEDF(xml_node edf_node, unsigned int *id, int level) {
  EDF* edf = new EDF(*id, level);

  #if _INFO==1
  cout << "Creating EDF " << *id << endl;
  #endif

  //iterate through all of the children nodes
  for (xml_node load = edf_node.first_child(); load; load = load.next_sibling()) {
    string type = load.attribute("type").as_string();

    *id = *id + 1;

    //If child is worker, parse a worker
    if( type == "worker" ) {
      parseWorker((ResourceAllocator*)edf, load, id);
    }
    //If child is scheduler, parse the correct scheduler
    else if( type == "scheduler" ) {
      parseScheduler((ResourceAllocator*)edf, load, id, level+1);
    }//end of scheduler
  }  

  return edf;
}

///This function receives a Scheduler and it call on the appropiate parsing function to return the full object
Scheduler* Parser::parseScheduler(ResourceAllocator* parent, xml_node sched_node, unsigned int *id, int level) {

  Scheduler *sched = NULL;
  string alg = sched_node.attribute("algorithm").as_string();

  //Call the appropiate parsing function
  if(alg == "TDMA") {
    sched = (Scheduler*) parseTDMA(sched_node, id, level);
  }
  else if(alg == "EDF") {
    sched = (Scheduler*) parseEDF(sched_node, id, level);
  }
  else {
    cout << "Parser error: '" << alg << "' algorithm was not recognized\n";
  }

  //set the parent
  if(sched != NULL) {
    sched->setParent(parent);
    sim->addThread((Thread*)sched);
  }

  return sched; 
}


///This function receives and TDMA node, and parses its load
TDMA* Parser::parseTDMA(xml_node tdma_node, unsigned int *id, int level) {

  TDMA *tdma = new TDMA(*id, level);

#if _INFO==1
    cout << "Creating TDMA " << *id << endl;
#endif

  //iterate through all of the children nodes
  for (xml_node load = tdma_node.first_child(); load; load = load.next_sibling()) {
    string type = load.attribute("type").as_string();

    *id = *id + 1;

    //If child is worker, parse a worker
    if( type == "worker" ) {
      Worker *w = parseWorker((ResourceAllocator*)tdma, load, id);

      tdma->add_load((Runnable*) w);
    }
    //If child is scheduler, parse the correct scheduler
    else if( type == "scheduler" ) {
      Scheduler* aux = parseScheduler((ResourceAllocator*)tdma, load, id, level+1);
      tdma->add_load(aux);
    }//end of scheduler

    //TODO: only iterate through children of type runnable
  }
  
  //TIME SLOTS
  xml_node time_slots = tdma_node.child("time_slots");
  for (xml_node slot = time_slots.first_child(); slot; slot = slot.next_sibling()) {
    tdma->add_slot(parseTime(slot));
  }

  return tdma;
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
    BusyWait *bw = new BusyWait(parseTime(worker_node.child("wcet")));

    worker = new Worker(parent, *id, busy_wait);
    worker->setTask(bw);
    d->setWorker(worker);
  }
  else if(load == "video") {
    Dispatcher *d = parseDispatcher(worker_node, id);
    *id = *id + 1;  

    #if _INFO==1
    cout << "Creating Worker " << *id << endl;
    #endif

    Video *vid = new Video();
    worker = new Worker(parent, *id, video);
    worker->setTask(vid);
    d->setWorker(worker);
  }
  else {
    cout << "Parser error: Worker " << *id << "'s load was not recognized\n";
  }

  if(worker!=NULL) {
    //Register thread with simulation object
    sim->addThread((Thread*)worker);
  }

  return worker;
}
