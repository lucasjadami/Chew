#ifndef RUNNER_H
#define RUNNER_H

#include "command.h"
#include "iohandler.h"

// the Runner class executes a command
class Runner
{

public:

	Runner();
	~Runner();
	
	int run(Command&, IOHandler&);
	
private:

};

#endif
