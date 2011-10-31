#ifndef JOBSHANDLER_H
#define JOBSHANDLER_H

#include "iohandler.h"

#include <unistd.h>
#include <signal.h>

using namespace std;

class JobsHandler
{

public:

	bool handleStop();
	bool handleInterrupt();
	void showJobs(IOHandler&);
	bool init(void (*)(int, siginfo_t*, void*));

private:

};

#endif
