#ifndef _PRIORITIES_H
#define _PRIORITIES_H

/********************************************************************************
 * CLASS DECLARATION
 ********************************************************************************
 */

class Priorities {
  public:
  //const int MAX_THREADS = _MAX_THREADS;

  static int get_main_pr();
  static int get_sched_pr(int level);
  static int get_disp_pr();
  static int get_server_pr(int x);
  static int get_active_pr();
  static int get_idle_pr();
  static int get_inactive_pr();    

};

#endif
