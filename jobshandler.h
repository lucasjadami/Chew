#ifndef JOBSHANDLER_H
#define JOBSHANDLER_H

#include "iohandler.h"

class JobsHandler
{

public:

	bool handleStop();
	bool handleInterrupt();
	void showJobs(IOHandler&);

private:

};

#endif
