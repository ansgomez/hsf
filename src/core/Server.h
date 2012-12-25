#ifndef _SERVER_H
#define _SERVER_H

#include "core/Runnable.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Server : public Runnable {
  public:
    void wrapper();

    void activate();

    void deactivate();

    void serve();

};

#endif
