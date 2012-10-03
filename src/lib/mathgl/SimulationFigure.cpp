#include "SimulationFigure.h"

/********************************************************************************
 * VARIABLES
 ********************************************************************************
 */

const char *plot_color[] = {"k", "b", "r", "g", "q", "c", "p", "h", "u", "P", "M"};

/********************************************************************************
 * CONSTRUCTORS AND METHODS
 ********************************************************************************
 */

//Constructor set some default values
SimulationFigure::SimulationFigure(string _path) {
  path = _path+"_figure.png";
  height = 0.75;
  width = 0.5;
  n_plots = 1;
  base = 0;
  plot_offset = 0;
  arrow_length = 1.3;
  gr = new mglGraph;
  x1=0; x2=1; y1=0; y2=1;
}

//Set the number of plots for the figure
void SimulationFigure::setNPlots(int n) {
  n_plots = n;
}

//Set the ranges of the figure
void SimulationFigure::setRanges(double x1, double x2, double y1, double y2) {
  this->x1 = x1;
  this->x2 = x2;
  this->y1 = y1;
  this->y2 = y2;
}

//Sets the size of the figure, as well as the title and x label
void SimulationFigure::setSize(int x, int y) {
  if (gr!=NULL) {
    gr->SetSize(x,y);
    gr->Title("HSF Simulation Results","", 6);
    gr->Label('x',"Time[ms]",0, 0);
  }
}

//Set the x domain of the current plot and the corresponding ranges of the figure
void SimulationFigure::setTimeRange(double t) {
  //Size depends on the range
  setSize((int)ceil(t)*UNIT_WIDTH, PLOT_HEIGHT*n_plots);
  setRanges(0,t,-0.25,1.5);
     
}

//Initialize a plot with y-label and default settings (ticks, ranges, etc)
void SimulationFigure::initPlot(int plot, char *name) { 
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
    gr->SetOrigin(0, base);
    //gr->Box("r");
    gr->Label('y'+'n', name, 1, 1);
    gr->SetTicks('x', 1, 0);
    gr->SetTicks('y', 1, 0);
    gr->Axis("xT");
  }
}

//This function draws an arrow indicating that the scheduler activated plot_i during this time
void SimulationFigure::drawSched(int plot, double sched_start, double sched_end, int clr) {
  //Assign plot color, unless it's overwritten
  //int color = (clr!=-1)? clr:plot+1;
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
    gr->Line(mglPoint(sched_start,base+height*1.1), mglPoint(sched_end,base+height*1.1), "KK");
	
  }
}

//This function draws a rectangle of a defined color starting at time t and a relative width
void SimulationFigure::drawJob(int plot, double start, double end, int clr) {
  //Assign plot color, unless it's overwritten
  int color = (clr!=-1)? clr:plot+1;
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
    gr->SetOrigin(0, base);
    //Draw Face
    gr->Face(mglPoint(start,base), mglPoint(end,base), mglPoint(start,base+height), mglPoint(end,base+height), plot_color[color]);
  }
}

//This function draws an arrival arrow at time t, with an option label
void SimulationFigure::drawArrival(int plot, double time, char *label, int color) {
  if(gr!=NULL) {
    gr->ColumnPlot(n_plots, plot, plot_offset);
    gr->SetRanges(x1,x2,y1,y2);
          
    //Draw Arrow
    gr->Line(mglPoint(time,base), mglPoint(time,base+height*arrow_length), "A2");
          
    //Draw Label
    float xpt[] = {time,NaN}; float ypt[] = {height*arrow_length, NaN};
    gr->Label(mglData(2, xpt), mglData(2, ypt), label);
  }
}

//This function exports the figure to a png file
void SimulationFigure::exportPNG() {
  if(gr!=NULL) {
    gr->WritePNG(path.data(),"",false);
    printf("Saved Simulation Figure\n");
  }
  else {
    printf("ERROR: Graph was null\n");
  }
}
