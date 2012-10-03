#ifndef _PARSER_H
#define _PARSER_H

#include "lib/xml/pugixml.hpp"

#include <string>
#include <time.h>

using namespace std;
using namespace pugi;

class Simulation;
class Scheduler;
class Worker;
class Dispatcher;

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

///This class takes care of parsing the XML file and instantiating all of the relevant objects.
class Parser {

 private:
  Simulation *sim;
  
  ///This function converts an XML "time" node to a timespec
  struct timespec parseTime(xml_node n);

  ///This function receives a TDMA node and parses the entire node to return the full object
  Scheduler* parseTDMA( xml_node sched, unsigned int *id, int level);

  ///This function receives a Worker node, and it returns the initialized worker object
  Worker* parseWorker(xml_node worker, unsigned int *id);
			     
  //This function extract information from an XML "worker" and returns its corresponding disp.
  Dispatcher* parseDispatcher(xml_node disp, unsigned int* id);
  
 public:  
  //Constructor needs simulation pointer
  Parser(Simulation *sim);

  ///This function receives the filepath and initiates the entire simulation
  void parseFile(string filePath);
};

#endif
