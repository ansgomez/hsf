#ifndef _VIDEO_H
#define _VIDEO_H

#include "core/Task.h"
#include "../lib/mjpeg/mjpeg.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Video : public Task {

  private:

  /*********** VARIABLES ***********/
  
  struct _mjpeg_state *state;

 public:

  /*********** CONSTRUCTOR ***********/

  ///The constructor requires a pointer to the simulation, its own dispatcher, and the WCET
  Video(Simulation *s, Dispatcher *disp);

  ///The destructor sinply call mjpeg_finish
  ~Video() {   mjpeg_finish(state); }

  /*********** INHERITED FUNCTIONS ***********/
  
  /**** FROM CRITERIA ****/
  
  //This function will decode one mjpeg frame
  void fire();
};
#endif
