#include "Periodic.h"

#include "Worker.h"
#include "Simulation.h"
#include "Trace.h"

#include <stdio.h>

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

Periodic::Periodic(Simulation *s, unsigned int id) : Dispatcher(s,id) {
  period.tv_sec = 0;
  period.tv_nsec = 10000000; //default period is 10 ms
}

void Periodic::dispatch() {
  struct timespec rem;

  while (sim->isSimulating() ==  1) {

#if _INFO == 1
    cout << "Disp " << disp_id << ": new job!\n";
#endif  

    sim->getTraces()->add_trace(dispatcher, disp_id, task_arrival);

    if(worker != NULL) {
      worker->new_job();
    }
    else {
      printf("Dispatcher error: worker is null!\n");
    }

    nanosleep(&period, &rem);
  }
}
