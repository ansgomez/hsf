#ifndef _CBS_H
#define _CBS_H

#include "servers/Server.h"
#include "queues/QueueCBS.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class CBS : public Server {

  private:

    QueueCBS queue;

  public:

    void replenish();

    void serve();

};
#endif
