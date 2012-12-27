#ifndef _SIMULATIONFIGURE_H
#define _SIMULATIONFIGURE_H

#include "mgl2/eval.h"
#include "mgl2/mgl.h"

#include "util/Enumerations.h"
#include "results/Trace.h"

#include <locale.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>


#if !defined(_MSC_VER) && !defined(__BORLANDC__)
#include <getopt.h>
#endif

using namespace std;

#define MAX_PLOTS    10  //Max number of column plot in figure
#define PLOT_HEIGHT  200 //Vert. Pixels per plot in picture
#define UNIT_WIDTH   2 //Hor. Pixels per time unit in picture


class SimulationFigure {
     
private:
  
  //Relative length of arrow (w.r.t. height)
  double      arrow_length;    

  //Vertical offset from x-axis
  double      base;            

  //Height of all job traces   
  double      height;          
  
  mglGraph   *gr;

  //Number of column plots in the figure
  int         n_plots;         

  //
  string      path;

  //Distance b/w colplots. in % of height
  double      plot_offset;    
  
  // Vector of Job trace
  vector<Trace> traces;
  
  //Width of one time unit
  double      width;          

  // voctor of work ids
  vector <unsigned int> worker_id;

  //Figure ranges
  double      x1, x2, y1, y2;  

public:

   SimulationFigure(string _path);

   void drawArrival(int plot, double time, char *label, int color=0);  
   void drawJob(int plot, double start, double end, int clr=-1);
   void drawSched(int plot, double sched_start, double sched_end, int clr=0);
   void exportEPS();
   void exportPNG();
   void exportSVG();
   void generateFigure();
   void importCSV(string name);
   void initPlot(int plot, char *name);
   void parseLine(string line);
   struct timespec parseTimespec(char *str);
   void printWorkerID();
   void setNPlots(int n);
   void setTimeRange(double t);  
   void setSize(int x, int y);
   void setRanges(double x1, double x2, double y1, double y2);
   void workerID_vector(unsigned int id);
};

#endif
