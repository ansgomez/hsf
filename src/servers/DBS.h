#ifndef _DBS_H
#define _DBS_H

#include "core/RunnableQueue.h"
#include "core/Server.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class DBS : public Server {

  private:

    RunnableQueue queue;

};
#endif
