#ifndef RUNNER_H
#define RUNNER_H

#include "command.h"
#include "iohandler.h"
#include "dirhandler.h"
#include "jobshandler.h"

// the Runner class executes a command
class Runner
{

public:

	Runner();
	~Runner();
	
	void runChain(vector<Command>&, IOHandler&, DirHandler&, JobsHandler&, bool);
	
private:

	int run(Command&, IOHandler&, DirHandler&, JobsHandler&, bool, bool);

	const char* one;
	const char* two;

};

#endif
