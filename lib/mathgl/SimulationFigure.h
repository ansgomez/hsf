#ifndef _SIMULATIONFIGURE_H
#define _SIMULATIONFIGURE_H

#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string>
#include <vector>
#include "mgl2/mgl.h"
#include "mgl2/eval.h"

#if !defined(_MSC_VER) && !defined(__BORLANDC__)
#include <getopt.h>
#endif

using namespace std;

#define MAX_PLOTS    10  //Max number of column plot in figure
#define UNIT_WIDTH   100 //Hor. Pixels per time unit in picture
#define PLOT_HEIGHT  300 //Vert. Pixels per plot in picture

class SimulationFigure {
     
private:
   mglGraph   *gr;
   double      width;           //Width of one time unit
   double      height;          //Height of all job traces
   double      arrow_length;    //Relative length of arrow (w.r.t. height)
   double      base;            //Vertical offset from x-axis     
   int         n_plots;         //Number of column plots in the figure
   double      x1, x2, y1, y2;  //Figure ranges
   double      plot_offset;     //Distance b/w colplots. in % of height
   string      path;

   void setSize(int x, int y);
   void setRanges(double x1, double x2, double y1, double y2);
      
public:
   SimulationFigure(string _path);
   void setNPlots(int n);
   void setTimeRange(double t);

   void initPlot(int plot, char *name);


   void drawSched(int plot, double sched_start, double sched_end, int clr=0);
   void drawJob(int plot, double start, double end, int clr=-1);
   void drawArrival(int plot, double time, char *label, int color=0);
   void exportPNG();
};

#endif
