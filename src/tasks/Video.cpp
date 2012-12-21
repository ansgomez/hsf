#include "Video.h"

#include "Dispatcher.h"

#include <iostream>

using namespace std;

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

/********************* CONSTRUCTOR *********************/

///The constructor requires a pointer to the simulation, its own dispatcher, and the WCET
Video::Video(Simulation *s, Dispatcher *disp) : Task(s, disp, video) {
  state = mjpeg_init();
}

/********************* MEMEBER FUNCTIONS *********************/

void Video::fire() {
  mjpeg_fire(state);
}
