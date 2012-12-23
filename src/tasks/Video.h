#ifndef _VIDEO_H
#define _VIDEO_H

#include "core/Task.h"
#include "lib/mjpeg/mjpeg.h"

class Video : public Task {
  /*********** VARIABLES ***********/
  private:
  struct _mjpeg_state *state;


  /*********** CONSTRUCTOR ***********/
 public:
    ///The constructor requires a pointer to the simulation, its own dispatcher, and the WCET
    Video(Simulation *s, Dispatcher *disp);

    ~Video() {   mjpeg_finish(state); }

  /*********** INHERITED FUNCTIONS ***********/
    //This function will decode one mjpeg frame
    void fire();
};
#endif
