#ifndef RUNNER_H
#define RUNNER_H

#include "command.h"
#include "iohandler.h"
#include "dirhandler.h"
#include "jobshandler.h"

#include <string>

using namespace std;

// the Runner class executes a command
class Runner
{

public:

	Runner();
	~Runner();
	
	void runChain(string, vector<Command>&, IOHandler&, DirHandler&, JobsHandler&, bool);
	
private:

	int run(Command&, IOHandler&, DirHandler&, JobsHandler&, bool, bool);

	const char* one;
	const char* two;

};

extern Runner runner;

#endif
