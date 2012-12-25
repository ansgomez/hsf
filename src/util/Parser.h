#ifndef _PARSER_H
#define _PARSER_H

#include "../lib/xml/pugixml.hpp"

#include <string>
#include <time.h>

class Dispatcher;
class Scheduler;
class Simulation;
class Worker;

using namespace std;
using namespace pugi;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

///This class takes care of parsing the XML file and instantiating all of the relevant objects.
class Parser {

 protected:

  /*********** VARIABLES ***********/

  ///Pointer to the simulation
  Simulation *sim;

 public:  
  
  /*********** CONSTRUCTOR ***********/

  ///Constructor needs simulation pointer
  Parser(Simulation *sim);

  /*********** MEMBER FUNCTIONS ***********/
  
  ///This function receives the filepath and initiates the entire simulation
  void parseFile(string filePath);

 private:

  /*********** MEMBER FUNCTIONS ***********/

  ///This function extract information from an XML "worker" and returns its corresponding disp.
  Dispatcher* parseDispatcher(xml_node disp, unsigned int* id);

  ///This function receives a TDMA node and parses the entire node to return the full object
  Scheduler* parseTDMA( xml_node sched, unsigned int *id, int level);

  ///This function converts an XML "time" node to a timespec
  struct timespec parseTime(xml_node n);

  ///This function receives a Worker node, and it returns the initialized worker object
  Worker* parseWorker(xml_node worker, unsigned int *id);

};

#endif
