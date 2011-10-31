#ifndef RUNNER_H
#define RUNNER_H

#include "command.h"
#include "iohandler.h"
#include "dirhandler.h"

// the Runner class executes a command
class Runner
{

public:

	Runner();
	~Runner();
	
	int run(Command&, IOHandler&, DirHandler&, bool, bool);
	
private:

	const char* one;
	const char* two;

};

#endif
