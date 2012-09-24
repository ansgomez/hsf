#include "Priorities.h"

///Maximum number of threads under a scheduler
#define MAX_THREADS 10

/********************************************************************************
 * CLASS DEFINITION
 ********************************************************************************
 */

int Priorities::get_main_pr() {
  return 6+MAX_THREADS;
}

int Priorities::get_sched_pr() {
  return 5+MAX_THREADS;
}

int Priorities::get_disp_pr() {
  return 4+MAX_THREADS;
}

int Priorities::get_server_pr(int x) {
  if ( x<0 || x>MAX_THREADS) {
    return -1;
  }
  else {
    return 4+x;
  }
}

int Priorities::get_active_pr() {
  return 3;
}

int Priorities::get_idle_pr() {
  return 2;
}

int Priorities::get_inactive_pr() {
  return 1;
}




