#include "results/results/Statistics.h"
#include "util/util/Enumerations.h"

#include <time.h>

int main() {
	struct timespec aux, aux2;

	//initialize first
	Statistics::initialize();

	//add results
	aux.tv_sec = 3;
	aux.tv_nsec = 10000;
	Statistics::addRuntime(idle, 0, aux);

	aux.tv_sec = 1;
	aux.tv_nsec = 200;
	Statistics::addRuntime(scheduler, 1, aux);

	aux.tv_sec = 2;
	aux.tv_nsec = 400;
	Statistics::addRuntime(worker, 2, aux);

	Statistics::addTrace(worker, 2, sched_start);
	usleep(1000);
	Statistics::addTrace(worker, 2, task_start);
	usleep(1000);
	Statistics::addTrace(worker, 2, task_end);
	usleep(1000);
	Statistics::addTrace(worker, 2, sched_end);
	usleep(1000);

	aux2.tv_sec = 8;
	aux2.tv_nsec = 10;
	Statistics::addMissedDeadline(2, aux, aux2);

	Statistics::addTrace(worker, 2, sched_start);
	usleep(1000);
	Statistics::addTrace(worker, 2, task_start);
	usleep(1000);
	Statistics::addTrace(worker, 2, task_end);
	usleep(1000);
	Statistics::addTrace(worker, 2, sched_end);

	aux2.tv_sec = 10;
	aux2.tv_nsec = 9000;
	Statistics::addMissedDeadline(2, aux, aux2);

	return 0;
}