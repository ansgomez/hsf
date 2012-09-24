#ifndef _PARSER_H
#define _PARSER_H


#include <string>
using namespace std;

class Simulation;
class Priorities;

///This class takes care of parsing the XML file and instantiating all of the relevant objects.
class Parser {
  public:
    void parseFile(string filePath, const Simulation * & sim);

};
#endif
