#ifndef JOBSHANDLER_H
#define JOBSHANDLER_H

#include "iohandler.h"

#include <vector>
#include <string>

using namespace std;

class JobsHandler
{

public:

	bool handleStop();
	bool handleInterrupt();
	void showJobs(IOHandler&);

private:

};

#endif
